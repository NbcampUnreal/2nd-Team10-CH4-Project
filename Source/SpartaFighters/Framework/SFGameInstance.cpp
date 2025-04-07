#include "Framework/SFGameInstance.h"

void USFGameInstance::LoadMapData()
{
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