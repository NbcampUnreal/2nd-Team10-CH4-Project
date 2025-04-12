#include "Framework/SFBattleGameMode.h"
#include "Framework/SFGameStateBase.h"
#include "GameFramework/Controller.h"
#include "Framework/SFPlayerState.h"

#include "Character/SFCharacter.h"

#include "LevelObject/SFCharacterSpawner.h"

#include "Kismet/GameplayStatics.h"
ASFBattleGameMode::ASFBattleGameMode()
{
	GameStateClass = ASFGameStateBase::StaticClass();
	PlayerStateClass = ASFPlayerState::StaticClass();
}

void ASFBattleGameMode::BeginPlay()
{
	Super::BeginPlay();

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
		}
	}
}

void ASFBattleGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	int32 Index = GameState->PlayerArray.IndexOfByKey(NewPlayer->PlayerState);
	ASFCharacterSpawner* StartSpawner = SpawnPoints.IsValidIndex(Index) ? SpawnPoints[Index] : nullptr;

	if (StartSpawner)
	{
		RestartPlayerAtTransform(NewPlayer, FTransform(StartSpawner->GetSpawnLocation()));
	}
	else
	{
		RequestRespawn(NewPlayer);
	}

	if (ASFPlayerState* PS = NewPlayer->GetPlayerState<ASFPlayerState>())
	{
		if (PS->GetSelectedCharacterRow().IsNone())
		{
			PS->SetSelectedCharacterRow("DefaultFighter"); 
		}
	}
}

APawn* ASFBattleGameMode::SpawnDefaultPawnFor_Implementation(AController* NewPlayer, AActor* StartSpot)
{
	ASFPlayerState* PS = NewPlayer->GetPlayerState<ASFPlayerState>();
	if (!PS || !CharacterDataTable)
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid PlayerState or CharacterDataTable"));
		return Super::SpawnDefaultPawnFor(NewPlayer, StartSpot);
	}

	const FString Context = TEXT("Character Context");
	FCharacterDataRow* Row = CharacterDataTable->FindRow<FCharacterDataRow>(PS->GetSelectedCharacterRow(), Context);
	if (!Row || !Row->CharacterClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("Character not found in DataTable or invalid class"));
		return Super::SpawnDefaultPawnFor(NewPlayer, StartSpot);
	}

	// 지정된 캐릭터 생성
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = NewPlayer;
	SpawnParams.Instigator = GetInstigator();
	FTransform SpawnTransform = StartSpot ? StartSpot->GetActorTransform() : FTransform::Identity;

	APawn* SpawnedPawn = GetWorld()->SpawnActor<APawn>(Row->CharacterClass, SpawnTransform, SpawnParams);
	if (!SpawnedPawn)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to spawn pawn for Player %s"), *NewPlayer->GetName());
	}
	return SpawnedPawn;
}

void ASFBattleGameMode::RequestRespawn(AController* DeadController)
{
	if (!DeadController) return;

	AActor* RespawnPoint = GetAvailableSpawnPoint(true);
	FTransform RespawnTransform = RespawnPoint ? RespawnPoint->GetActorTransform() : FTransform::Identity;

	APawn* NewPawn = SpawnDefaultPawnFor(DeadController, RespawnPoint);
	if (NewPawn)
	{
		DeadController->UnPossess();
		DeadController->Possess(NewPawn);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Respawn failed for controller %s"), *DeadController->GetName());
	}
}

AActor* ASFBattleGameMode::GetAvailableSpawnPoint(bool bForRespawn) const
{
	TArray<ASFCharacterSpawner*> Filtered = SpawnPoints.FilterByPredicate(
		[bForRespawn](ASFCharacterSpawner* Spawner)
		{
			return Spawner && Spawner->bIsRespawnPoint == bForRespawn;
		});

	if (Filtered.Num() == 0) return nullptr;

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