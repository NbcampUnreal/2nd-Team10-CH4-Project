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
	UE_LOG(LogTemp, Warning, TEXT("--- [SpawnAICharacters] Function Entered ---")); // 함수 진입 로그

	// 서버 권한 확인
	if (!HasAuthority())
	{
		UE_LOG(LogTemp, Error, TEXT("[SpawnAICharacters] Error: Not running on Authority! AI can only be spawned by the server."));
		return;
	}

	// 1. AI 클래스 유효성 검사
	if (!AICharacterClass)
	{
		// 이 로그가 출력되면 블루프린트에서 AICharacterClass 설정이 안 된 것!
		UE_LOG(LogTemp, Error, TEXT("[SpawnAICharacters] Error: AICharacterClass is NOT set in GameMode! Check the GameMode Blueprint Defaults."));
		return;
	}
	else
	{
		// 설정된 클래스 이름 로그 (디버깅에 유용)
		UE_LOG(LogTemp, Log, TEXT("[SpawnAICharacters] AICharacterClass is set to: %s"), *AICharacterClass->GetName());
	}

	// 2. 스폰 포인트 유효성 검사
	UE_LOG(LogTemp, Log, TEXT("[SpawnAICharacters] Checking SpawnPoints. Current Count: %d"), SpawnPoints.Num());
	if (SpawnPoints.Num() == 0)
	{
		// 스폰 포인트가 하나도 없으면 스폰 불가
		UE_LOG(LogTemp, Error, TEXT("[SpawnAICharacters] Error: No spawn points found in the SpawnPoints array! Add ASFCharacterSpawner actors to the level."));
		return;
	}

	// 3. 스폰 루프 시작
	UE_LOG(LogTemp, Log, TEXT("[SpawnAICharacters] Starting loop to attempt spawning %d AI characters."), NumAICharactersToSpawn);

	int32 SuccessfullySpawnedCount = 0; // 성공적으로 스폰된 AI 수 추적

	for (int32 i = 0; i < NumAICharactersToSpawn; ++i)
	{
		UE_LOG(LogTemp, Log, TEXT("[SpawnAICharacters] --- Attempting to spawn AI #%d ---"), i + 1);

		// 4. 스폰 포인트 찾기
		ASFCharacterSpawner* SelectedSpawner = nullptr;
		int32 Attempts = 0;
		const int32 MaxAttempts = SpawnPoints.Num() * 2; // 스폰 포인트 수의 2배만큼 시도 (무한 루프 방지 강화)

		// 이미 사용된 스폰 포인트를 제외하고 찾거나, 다른 방식으로 선택 (플레이어 위치와 먼 곳 등)
		// 여기서는 간단히 랜덤 선택 + 유효성 검사
		while (Attempts < MaxAttempts)
		{
			int32 RandomIndex = FMath::RandRange(0, SpawnPoints.Num() - 1);
			if (SpawnPoints.IsValidIndex(RandomIndex) && IsValid(SpawnPoints[RandomIndex])) // 인덱스 및 스포너 유효성 확인
			{
				SelectedSpawner = SpawnPoints[RandomIndex];
				UE_LOG(LogTemp, Log, TEXT("[SpawnAICharacters] Found potential spawner %s at index %d."), *SelectedSpawner->GetName(), RandomIndex);
				// TODO: 이 스포너가 이미 점유되었는지 확인하는 로직 추가 가능
				break; // 유효한 스포너 찾으면 루프 종료
			}
			Attempts++;
		}

		// 스폰 포인트 찾기 실패 시
		if (!SelectedSpawner)
		{
			UE_LOG(LogTemp, Error, TEXT("[SpawnAICharacters] Error: Failed to find a valid spawn point for AI #%d after %d attempts. Skipping this AI."), i + 1, MaxAttempts);
			continue; // 다음 AI 스폰 시도
		}

		// 5. 스폰 파라미터 설정
		FTransform SpawnTransform = SelectedSpawner->GetActorTransform();
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this; // GameMode가 AI의 오너
		SpawnParams.Instigator = nullptr; // AI는 보통 Instigator 없음
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn; // 충돌 시 위치 조정

		// 6. 스폰 시도 로그
		UE_LOG(LogTemp, Log, TEXT("[SpawnAICharacters] Attempting to spawn AI %d (%s) at %s using Spawner %s"),
			i + 1,
			*AICharacterClass->GetName(),
			*SpawnTransform.ToString(),
			*SelectedSpawner->GetName());

		// 7. **** 실제 AI 캐릭터 스폰 ****
		ACharacter* NewAICharacter = GetWorld()->SpawnActor<ACharacter>(AICharacterClass, SpawnTransform, SpawnParams);

		// 8. 스폰 결과 확인
		if (IsValid(NewAICharacter)) // 스폰 성공 시 IsValid 로 확인
		{
			SuccessfullySpawnedCount++;
			UE_LOG(LogTemp, Warning, TEXT("[SpawnAICharacters] SUCCESS: AI Character #%d spawned: %s (Class: %s)"),
				SuccessfullySpawnedCount, *NewAICharacter->GetName(), *NewAICharacter->GetClass()->GetName());

			// 9. **** AI 컨트롤러 스폰 시도 (매우 중요!) ****
			UE_LOG(LogTemp, Log, TEXT("[SpawnAICharacters] Attempting to spawn default controller for AI: %s"), *NewAICharacter->GetName());
			NewAICharacter->SpawnDefaultController(); // AI 캐릭터 클래스에 지정된 기본 컨트롤러 스폰 시도

			// 10. 컨트롤러 스폰 결과 확인
			AController* AIController = NewAICharacter->GetController(); // 스폰된 컨트롤러 가져오기
			if (IsValid(AIController))
			{
				UE_LOG(LogTemp, Log, TEXT("[SpawnAICharacters] SUCCESS: Controller %s spawned and possessed AI %s."),
					*AIController->GetName(), *NewAICharacter->GetName());
				// 컨트롤러가 비헤이비어 트리를 실행하도록 추가 설정 필요 시 여기에
				// 예: AYourAIController* MyAICon = Cast<AYourAIController>(AIController); if(MyAICon) { MyAICon->RunBehaviorTree(MyBTAsset); }
			}
			else
			{
				// 컨트롤러 스폰 실패 시 AI는 동작하지 않을 가능성이 높음!
				UE_LOG(LogTemp, Error, TEXT("[SpawnAICharacters] FAILURE: Failed to spawn or possess default controller for AI %s! Check AI Character's 'AIController Class' setting in Class Defaults."),
					*NewAICharacter->GetName());
			}
		}
		else // 스폰 실패 시
		{
			UE_LOG(LogTemp, Error, TEXT("[SpawnAICharacters] FAILURE: GetWorld()->SpawnActor failed for AI #%d using class %s at %s!"),
				i + 1, *AICharacterClass->GetName(), *SpawnTransform.ToString());
			// 실패 원인 추적 필요 (콜리전, 클래스 문제 등)
		}
	} // End of for loop

	UE_LOG(LogTemp, Warning, TEXT("--- [SpawnAICharacters] Function Exited. Successfully spawned %d out of %d requested AI characters. ---"), SuccessfullySpawnedCount, NumAICharactersToSpawn);
}