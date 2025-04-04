#include "Framework/SFGameInstanceSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "Framework/SFGameModeBase.h"

void USFGameInstanceSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	CurrentGameState = EGameState::LoginMenu;
	CurrentGameMode = EGameMode::Cooperative;
	CurrentGameModeInstance = nullptr;
}

void USFGameInstanceSubsystem::Deinitialize()
{
	Super::Deinitialize();
	CurrentGameModeInstance = nullptr;
}

void USFGameInstanceSubsystem::SetCurrentGameState(EGameState NewGameState)
{
	if (CurrentGameState != NewGameState)
	{
		CurrentGameState = NewGameState;
		UE_LOG(LogTemp, Log, TEXT("Game State changed to: %s"), *UEnum::GetValueAsString(CurrentGameState));
	}
}

void USFGameInstanceSubsystem::SwitchGameMode(EGameMode NewGameMode)
{
    if (CurrentGameMode != NewGameMode)
    {
        CurrentGameMode = NewGameMode;
        UE_LOG(LogTemp, Log, TEXT("Game Mode Switched: %d"), static_cast<int32>(NewGameMode));

        if (CurrentGameModeInstance)
        {
            CurrentGameModeInstance->ConditionalBeginDestroy();
            CurrentGameModeInstance = nullptr;
        }

        UWorld* World = GetWorld();
        if (World)
        {
            CurrentGameModeInstance = Cast<ASFGameModeBase>(UGameplayStatics::GetGameMode(World));
        }
    }
}