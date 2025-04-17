#include "Framework/SFBattleGameMode.h"
#include "Framework/SFGameStateBase.h"
#include "Framework/SFGameInstance.h"
#include "Framework/SFPlayerState.h"
#include "Framework/SFPlayerController.h"
#include "GameFramework/Controller.h"

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

	NumAICharactersToSpawn = 2;
	BattleStartDelay = 3.f;
	BattleTime = 180.f;
	ReturnToLobbyTime = 6.f;
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

	if (ASFGameStateBase* SFGameState = GetGameState<ASFGameStateBase>())
	{
		SFGameState->SetBattleDuration(BattleTime);
	}
}

void ASFBattleGameMode::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
	Super::HandleStartingNewPlayer_Implementation(NewPlayer);
	UE_LOG(LogTemp, Warning, TEXT("ASFBattleGameMode::HandleStartingNewPlayer_Implementation"));

	APlayerState* PS = NewPlayer->PlayerState;
	UE_LOG(LogTemp, Warning, TEXT("HandleStartingNewPlayer_Implementation: Controller = %s, PlayerState = %s"),
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
	UE_LOG(LogTemp, Error, TEXT("GetSelectedCharacterRow : %s"), *PS->GetSelectedCharacterRow().ToString());
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

	int32 Index = GameState->PlayerArray.IndexOfByKey(PS);
	ASFCharacterSpawner* StartSpawner = SpawnPoints.IsValidIndex(Index) ? SpawnPoints[Index] : nullptr;
	FTransform SpawnTransform = StartSpawner ? FTransform(StartSpawner->GetSpawnLocation()) : FTransform::Identity;

	APawn* NewPawn = GetWorld()->SpawnActor<APawn>(Row->CharacterClass, SpawnTransform);
	ASFPlayerController* SFPlayerController = Cast<ASFPlayerController>(PC);
	if (NewPawn)
	{
		UE_LOG(LogTemp, Error, TEXT("NewPawn : %s"), *NewPawn->GetActorNameOrLabel());
		UE_LOG(LogTemp, Warning, TEXT("Before Possess: PC->GetPawn(): %s"),
			(SFPlayerController->GetPawn() ? *SFPlayerController->GetPawn()->GetName() : TEXT("None")));

		SFPlayerController->Possess(NewPawn);
		UE_LOG(LogTemp, Warning, TEXT("After Possess: PC->GetPawn(): %s"),
			(SFPlayerController->GetPawn() ? *SFPlayerController->GetPawn()->GetName() : TEXT("None")));
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
	SpawnAICharacters();

	UE_LOG(LogTemp, Log, TEXT("Battle Started"));

	GetWorldTimerManager().SetTimer(
		BattleTimerHandle,
		this,
		&ASFBattleGameMode::EndBattle,
		BattleTime,
		false);

	if (ASFGameStateBase* SFGameState = GetGameState<ASFGameStateBase>())
	{
		SFGameState->SetBattleStartTime(GetWorld()->GetTimeSeconds());
		/*SFGameState->SetBattleDuration(BattleTime);*/
		SFGameState->SetReturnToLobbyTime(ReturnToLobbyTime);
	}

	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		if (ASFPlayerController* SFPlayerController = Cast<ASFPlayerController>(*It))
		{
			// This is Server PlayerController
			// But, Client RPC is this called.. Go Client..
			SFPlayerController->Client_StartHUDUpdate();
		}
	}

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

	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		if (ASFPlayerController* SFPlayerController = Cast<ASFPlayerController>(*It))
		{
			// This is Server PlayerController
			// But, Client RPC is this called.. Go Client..
			SFPlayerController->Client_EndBattle();
		}
	}

	ASFPlayerState* Winner = CalculateWinner();
	ASFGameStateBase* BattleGameState = GetGameState<ASFGameStateBase>();

	if (BattleGameState)
	{
		BattleGameState->SetWinner(Winner);
		BattleGameState->SetStartReturnToLobbyTime(GetWorld()->GetTimeSeconds());
	}

	GetWorldTimerManager().SetTimer(
		ReturnLobbyTimerHandle,
		this, &ASFBattleGameMode::ReturnToLobby,
		ReturnToLobbyTime,
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
	UE_LOG(LogTemp, Log, TEXT("=============Returning all players to lobby=============="));

	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		if (ASFPlayerController* SFPlayerController = Cast<ASFPlayerController>(*It))
		{
			SFPlayerController->Client_EndReturnToLobbyTimer();
			//SFPlayerController->Client_TravelToLobby();
		}
	}

	GetWorldTimerManager().SetTimer(
		LazyServerTravelTimerHandle,
		this, 
		&ASFBattleGameMode::ServerTravelToLobby,
		0.5f,
		false);	
}

void ASFBattleGameMode::ServerTravelToLobby()
{
	GetWorld()->ServerTravel("/Game/SpartaFighters/Level/Menu/LobbyMenu", true);
}

void ASFBattleGameMode::SpawnAICharacters()
{    
	if (!HasAuthority())
	{
		UE_LOG(LogTemp, Error, TEXT("[SpawnAICharacters] Error: Not running on Authority! AI can only be spawned by the server."));
		return;
	}

	if (!AICharacterClass)
	{
		UE_LOG(LogTemp, Error, TEXT("[SpawnAICharacters] Error: AICharacterClass is NOT set in GameMode! Check the GameMode Blueprint Defaults."));
		return;
	}

	if (SpawnPoints.Num() == 0)
	{
		UE_LOG(LogTemp, Error, TEXT("[SpawnAICharacters] Error: No spawn points found in the SpawnPoints array! Add ASFCharacterSpawner actors to the level."));
		return;
	}

	int32 SuccessfullySpawnedCount = 0;

	for (int32 i = 0; i < NumAICharactersToSpawn; ++i)
	{
		ASFCharacterSpawner* SelectedSpawner = nullptr;
		int32 Attempts = 0;
		const int32 MaxAttempts = SpawnPoints.Num() * 2;

		while (Attempts < MaxAttempts)
		{
			int32 RandomIndex = FMath::RandRange(0, SpawnPoints.Num() - 1);
			if (SpawnPoints.IsValidIndex(RandomIndex) && IsValid(SpawnPoints[RandomIndex]))
			{
				SelectedSpawner = SpawnPoints[RandomIndex];
				break;
			}
			Attempts++;
		}

		if (!SelectedSpawner)
		{
			UE_LOG(LogTemp, Error, TEXT("[SpawnAICharacters] Error: Failed to find a valid spawn point for AI #%d after %d attempts. Skipping this AI."), i + 1, MaxAttempts);
			continue;
		}

		FTransform SpawnTransform = SelectedSpawner->GetActorTransform();
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = nullptr;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

		ACharacter* NewAICharacter = GetWorld()->SpawnActor<ACharacter>(AICharacterClass, SpawnTransform, SpawnParams);
		if (IsValid(NewAICharacter))
		{
			SuccessfullySpawnedCount++;

			NewAICharacter->SpawnDefaultController();

			AController* AIController = NewAICharacter->GetController();
			if (IsValid(AIController))
			{
				UE_LOG(LogTemp, Log, TEXT("[SpawnAICharacters] SUCCESS: Controller %s spawned and possessed AI %s."),
					*AIController->GetName(), *NewAICharacter->GetName());
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("[SpawnAICharacters] FAILURE: Failed to spawn or possess default controller for AI %s! Check AI Character's 'AIController Class' setting in Class Defaults."),
					*NewAICharacter->GetName());
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("[SpawnAICharacters] FAILURE: GetWorld()->SpawnActor failed for AI #%d using class %s at %s!"),
				i + 1, *AICharacterClass->GetName(), *SpawnTransform.ToString());
		}
	}
}