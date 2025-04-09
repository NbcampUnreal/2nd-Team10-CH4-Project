#include "Framework/SFCheatManager.h"
#include "Framework/SFGameInstance.h"
#include "Framework/SFGameInstanceSubsystem.h"

void USFCheatManager::Cheat_ChangeMap(const FString& MapName)
{
    UE_LOG(LogTemp, Warning, TEXT("Cheat_ChangeMap called with: %s"), *MapName);

    if (UWorld* World = GetWorld())
    {
        if (USFGameInstance* GameInstance = Cast<USFGameInstance>(World->GetGameInstance()))
        {
            if (USFGameInstanceSubsystem* Subsystem = GameInstance->GetSubsystem<USFGameInstanceSubsystem>())
            {
                Subsystem->ChangeLevelByMapName(MapName);
            }
            else
            {
                UE_LOG(LogTemp, Error, TEXT("GameInstanceSubsystem not found"));
            }
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("Invalid GameInstance cast"));
        }
    }
}
