#include "Framework/SFCooperativeGameMode.h"
#include "Framework/SFGameStateBase.h"
#include "Framework/SFPlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "LevelObject/SFCharacterSpawner.h"
#include "Framework/SFPlayerController.h"
#include "DataTable/CharacterDataRow.h"

ASFCooperativeGameMode::ASFCooperativeGameMode()
{
	UE_LOG(LogTemp, Warning, TEXT("Cooperative Game Mode Initialized"));
	GameStateClass = ASFGameStateBase::StaticClass();
	PlayerStateClass = ASFPlayerState::StaticClass();

	static ConstructorHelpers::FObjectFinder<UDataTable> DT_CharacterData(TEXT("/Game/SpartaFighters/DataAsset/DataTable/DT_CharacterData.DT_CharacterData"));
	if (DT_CharacterData.Succeeded())
	{
		CharacterDataTable = DT_CharacterData.Object;
	}
}

void ASFCooperativeGameMode::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Warning, TEXT("ASFCoopGameMode::BeginPlay"));

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

void ASFCooperativeGameMode::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
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
			&ASFCooperativeGameMode::PollCharacterSpawnRequests,
			0.5f,
			true
		);
	}
}

void ASFCooperativeGameMode::PollCharacterSpawnRequests()
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

void ASFCooperativeGameMode::HandleCharacterSpawnRequest(APlayerController* PC)
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
		UE_LOG(LogTemp, Warning, TEXT("Before Possess: PC->GetPawn(): %s"),
			(SFPlayerController->GetPawn() ? *SFPlayerController->GetPawn()->GetName() : TEXT("None")));

		SFPlayerController->Possess(NewPawn);
		UE_LOG(LogTemp, Warning, TEXT("After Possess: PC->GetPawn(): %s"),
			(SFPlayerController->GetPawn() ? *SFPlayerController->GetPawn()->GetName() : TEXT("None")));
	}
}