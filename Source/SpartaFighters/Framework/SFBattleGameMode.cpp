#include "Framework/SFBattleGameMode.h"
#include "Framework/SFGameStateBase.h"
#include "GameFramework/Controller.h"
#include "Framework/SFPlayerState.h"
#include "Framework/SFPlayerController.h"

#include "Character/SFCharacter.h"

#include "LevelObject/SFCharacterSpawner.h"

#include "Kismet/GameplayStatics.h"
ASFBattleGameMode::ASFBattleGameMode()
{
	GameStateClass = ASFGameStateBase::StaticClass();
	PlayerStateClass = ASFPlayerState::StaticClass();

	static ConstructorHelpers::FObjectFinder<UDataTable> DT_CharacterData(TEXT("/Game/SpartaFighters/DataAsset/DataTable/DT_CharacterData.DT_CharacterData"));
	if (DT_CharacterData.Succeeded())
	{
		CharacterDataTable = DT_CharacterData.Object;
	}
}

void ASFBattleGameMode::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Warning, TEXT("ASFBattleGameMode::BeginPlay"));
	GetWorldTimerManager().SetTimer(
		BattleStartTimerHandle,
		this,
		&ASFBattleGameMode::StartBattle,
		BattleStartDelay,
		false);

	TArray<AActor*> FoundSpawners;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASFCharacterSpawner::StaticClass(), FoundSpawners);

	for (AActor* Actor : FoundSpawners)
	{
		if (ASFCharacterSpawner* Spawner = Cast<ASFCharacterSpawner>(Actor))
		{
			SpawnPoints.Add(Spawner);
			UE_LOG(LogTemp, Warning, TEXT("Spawner Added to SpawnPoints"));
		}
	}
}

void ASFBattleGameMode::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
	Super::HandleStartingNewPlayer_Implementation(NewPlayer);
	UE_LOG(LogTemp, Warning, TEXT("ASFBattleGameMode::HandleStartingNewPlayer_Implementation"));

	APlayerState* PS = NewPlayer->PlayerState;
	UE_LOG(LogTemp, Warning, TEXT("PostLogin: Controller = %s, PlayerState = %s"),
		*NewPlayer->GetName(),
		PS ? *PS->GetName() : TEXT("nullptr"));


	if (NewPlayer)
	{
		bWaitingForSpawn.Add(NewPlayer, true);

		GetWorldTimerManager().SetTimer(
			SpawnPollingTimerHandle,
			this,
			&ASFBattleGameMode::PollCharacterSpawnRequests,
			0.5f,
			true 
		);
	}
}

void ASFBattleGameMode::PollCharacterSpawnRequests()
{
	TArray<APlayerController*> ControllersToRemove;

	for (auto& Elem : bWaitingForSpawn)
	{
		APlayerController* PC = Elem.Key;
		if (!IsValid(PC))
		{

			continue;
		}

		ASFPlayerState* PS = PC->GetPlayerState<ASFPlayerState>();
		if (PS && PS->GetSelectedCharacterRow() != NAME_None)
		{
			UE_LOG(LogTemp, Log, TEXT("Spawning character for PC: %s"), *PC->GetName());
			HandleCharacterSpawnRequest(PC);
			ControllersToRemove.Add(PC);
		}
		else
		{
			UE_LOG(LogTemp, Log, TEXT("Waiting for valid SelectedCharacterRow for PC: %s"), *PC->GetName());
		}
	}

	for (APlayerController* PC : ControllersToRemove)
	{
		bWaitingForSpawn.Remove(PC);
	}

	if (bWaitingForSpawn.Num() == 0)
	{
		GetWorldTimerManager().ClearTimer(SpawnPollingTimerHandle);
	}
}

void ASFBattleGameMode::RequestRespawn(AController* DeadController)
{
	if (!DeadController)
	{
		return;
	}

	APlayerController* PC = Cast<APlayerController>(DeadController);
	if (!PC)
	{
		return;
	}

	HandleCharacterSpawnRequest(PC);
}

void ASFBattleGameMode::HandleCharacterSpawnRequest(APlayerController* PC)
{
	UE_LOG(LogTemp, Error, TEXT("====HandleCharacterSpawnRequest===="));
	if (!PC)
	{
		return;
	}
	ASFPlayerState* PS = PC->GetPlayerState<ASFPlayerState>();
	if (!PS || !CharacterDataTable)
	{
		return;
	}
	UE_LOG(LogTemp, Error, TEXT("ASFPlayerState : %s"), *PS->GetActorNameOrLabel());

	const FString Context = TEXT("Character Context");
	FCharacterDataRow* Row = CharacterDataTable->FindRow<FCharacterDataRow>(PS->GetSelectedCharacterRow(), Context);
	if (!Row)
	{
		UE_LOG(LogTemp, Error, TEXT("nullptr : Row"));
		return;
	}
	if (!Row->CharacterClass)
	{
		UE_LOG(LogTemp, Error, TEXT("nullptr : Row->CharacterClass"));
		return;
	}
	UE_LOG(LogTemp, Error, TEXT("GetSelectedCharacterRow : %s"), *PS->GetSelectedCharacterRow().ToString());

	int32 Index = GameState->PlayerArray.IndexOfByKey(PS);
	ASFCharacterSpawner* StartSpawner = SpawnPoints.IsValidIndex(Index) ? SpawnPoints[Index] : nullptr;
	FTransform SpawnTransform = StartSpawner ? FTransform(StartSpawner->GetSpawnLocation()) : FTransform::Identity;

	APawn* NewPawn = GetWorld()->SpawnActor<APawn>(Row->CharacterClass, SpawnTransform);
	ASFPlayerController* SFPlayerController = Cast<ASFPlayerController>(PC);
	if (NewPawn)
	{
		UE_LOG(LogTemp, Error, TEXT("NewPawn : %s"), *NewPawn->GetActorNameOrLabel());
		SFPlayerController->Possess(NewPawn);
	}
}

AActor* ASFBattleGameMode::GetAvailableSpawnPoint(bool bForRespawn) const
{
	TArray<ASFCharacterSpawner*> Filtered = SpawnPoints.FilterByPredicate(
		[bForRespawn](ASFCharacterSpawner* Spawner)
		{
			return Spawner && Spawner->bIsRespawnPoint == bForRespawn;
		});

	if (Filtered.Num() == 0)
	{
		return nullptr;
	}

	return Filtered[FMath::RandRange(0, Filtered.Num() - 1)];
}

void ASFBattleGameMode::StartBattle()
{
	UE_LOG(LogTemp, Log, TEXT("Battle Started"));

	GetWorldTimerManager().SetTimer(
		BattleTimerHandle,
		this,
		&ASFBattleGameMode::EndBattle,
		BattleTime,
		false);

	Multicast_StartBattle();
}

void ASFBattleGameMode::Multicast_StartBattle_Implementation()
{
	// TODO : UI or Battle Start Animations
	UE_LOG(LogTemp, Log, TEXT("Clients received battle start signal"));
}

void ASFBattleGameMode::HandlePlayerDeath(AController* DeadController)
{
	ensureAlways(false);
	// TODO : Update Logic in ASFCharacter::OnDeath/Die/
	//if (HasAuthority())
	//{
	//    AController* OwnerController = GetController();
	//    if (ASFBattleGameMode* GM = GetWorld()->GetAuthGameMode<ASFBattleGameMode>())
	//    {
	//        GM->HandlePlayerDeath(OwnerController);
	//    }

	//    Destroy(); // 죽은 캐릭터 제거
	//}
	if (!DeadController)
	{
		return;
	}

	ASFPlayerState* DeadPS = DeadController->GetPlayerState<ASFPlayerState>();
	if (DeadPS)
	{
		DeadPS->AddDeathCount();
	}

	RequestRespawn(DeadController);
}

void ASFBattleGameMode::EndBattle()
{
	UE_LOG(LogTemp, Log, TEXT("Battle Ended"));

	ASFPlayerState* Winner = CalculateWinner();
	ASFGameStateBase* BattleGameState = GetGameState<ASFGameStateBase>();

	if (BattleGameState)
	{
		BattleGameState->SetWinner(Winner);
	}

	GetWorldTimerManager().SetTimer(
		ReturnLobbyTimerHandle,
		this, &ASFBattleGameMode::ReturnToLobby,
		5.0f,
		false);
}

ASFPlayerState* ASFBattleGameMode::CalculateWinner()
{
	ASFPlayerState* Winner = nullptr;
	int32 LowestDeath = MAX_int32;

	for (APlayerState* PS : GameState->PlayerArray)
	{
		ASFPlayerState* SFPS = Cast<ASFPlayerState>(PS);
		if (SFPS && SFPS->GetDeathCount() < LowestDeath)
		{
			LowestDeath = SFPS->GetDeathCount();
			Winner = SFPS;
		}
	}

	return Winner;
}

void ASFBattleGameMode::ReturnToLobby()
{
	UE_LOG(LogTemp, Log, TEXT("Returning all players to lobby"));

	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		if (APlayerController* PC = It->Get())
		{
			Client_TravelToLobby(PC);
		}
	}
}

void ASFBattleGameMode::Client_TravelToLobby_Implementation(APlayerController* PC)
{
	if (PC)
	{
		PC->ClientTravel(TEXT("/Game/SpartaFighters/Level/LobbyMenu"), ETravelType::TRAVEL_Absolute);
	}
}