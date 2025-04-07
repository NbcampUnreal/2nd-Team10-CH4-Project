#include "SFGameInstanceSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "Framework/SFGameModeBase.h"
#include "Framework/SFSingleGameMode.h"
#include "Framework/SFBattleGameMode.h"
#include "Framework/SFCooperativeGameMode.h"
#include "DataTypes/GameModeType.h"
#include "UI/UIManager/UIManager.h"

void USFGameInstanceSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

    UIManager = NewObject<UUIManager>(GetGameInstance());

    if (!MapDataTable)
    {
        UE_LOG(LogTemp, Warning, TEXT("MapDataTable is not assigned in USFGameInstanceSubsystem."));
        return;
    }

    static const FString ContextString(TEXT("Map Lookup"));
    TArray<FMapInfoRow*> AllMaps;
    MapDataTable->GetAllRows<FMapInfoRow>(ContextString, AllMaps);

    if (AllMaps.Num() > 0)
    {
        CurrentMapInfoRow = *AllMaps[0];
        UE_LOG(LogTemp, Log, TEXT("Loaded first map from DataTable: %s"), *CurrentMapInfoRow.MapInfo.MapName);
    }
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

void USFGameInstanceSubsystem::ChangeLevelByMapID(int32 MapID)
{
    if (!MapDataTable)
    {
        UE_LOG(LogTemp, Warning, TEXT("MapDataTable is not assigned."));
        return;
    }

	static const FString ContextString(TEXT("MapInfo Context"));
    TArray<FMapInfoRow*> AllMaps;
    MapDataTable->GetAllRows<FMapInfoRow>(ContextString, AllMaps);

    for (FMapInfoRow* MapRow : AllMaps)
    {
        if (MapRow && MapRow->MapID == MapID)
        {
            FString LevelToLoad = MapRow->MapInfo.LevelPath;

            UWorld* World = GetWorld();
            if (!World)
            {
                UE_LOG(LogTemp, Error, TEXT("GetWorld() returned nullptr."));
                return;
            }

            APlayerController* PC = World->GetFirstPlayerController();
            if (!PC)
            {
                UE_LOG(LogTemp, Error, TEXT("No PlayerController found in the world."));
                return;
            }

            if (IsRunningDedicatedServer())
            {
                UE_LOG(LogTemp, Log, TEXT("Changing Level to: %s (Server)"), *LevelToLoad);
                UGameplayStatics::OpenLevel(World, FName(*LevelToLoad), true);
            }
            else
            {
                UE_LOG(LogTemp, Log, TEXT("Changing Level to: %s (Client)"), *LevelToLoad);
                PC->ClientTravel(LevelToLoad, ETravelType::TRAVEL_Absolute);
            }
            return;
        }
    }

    UE_LOG(LogTemp, Warning, TEXT("Map not found for ID: %d"), MapID);
}
