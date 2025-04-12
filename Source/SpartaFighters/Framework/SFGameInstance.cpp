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
    MapDataTable->GetAllRows(ContextString, AllMaps);

    for (FMapInfoRow* MapInfoRow : AllMaps)
    {
        if (MapInfoRow)
        {
            MapInfoRow->MapID = GetTypeHash(MapInfoRow->MapInfo.MapName);
            UE_LOG(LogTemp, Log, TEXT("Loaded map: %s"), *MapInfoRow->MapInfo.MapName);
        }
    }
}

//void USFGameInstance::LoadCharacterData()
//{
//    if (!CharacterDataTable)
//    {
//        UE_LOG(LogTemp, Warning, TEXT("CharacterDataTable is not assigned in USFGameInstance."));
//        return;
//    }
//
//    static const FString ContextString(TEXT("Character Lookup"));
//    TArray<FCharacterDataRow*> AllCharacters;
//    CharacterDataTable->GetAllRows(ContextString, AllCharacters);
//
//    for (FCharacterDataRow* CharacterInfoRow : AllCharacters)
//    {
//        if (CharacterInfoRow)
//        {
//            const int32 ID = GetTypeHash(CharacterInfoRow->CharacterName.ToString());
//            UE_LOG(LogTemp, Log, TEXT("Loaded Character: %s (Hash: %d)"), *CharacterInfoRow->CharacterName.ToString(), ID);
//        }
//    }
//}