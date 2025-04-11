#include "Framework/SFCheatManager.h"
#include "Framework/SFGameInstance.h"
#include "Framework/SFGameInstanceSubsystem.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"
#include "Engine/Engine.h"

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

void USFCheatManager::PrintServerInfo()
{
    APlayerController* PC = GetOuterAPlayerController();
    if (!PC)
    {
        UE_LOG(LogTemp, Warning, TEXT("NULL : GetOuterAPlayerController"));
        return;
    }

    UWorld* World = PC->GetWorld();
    if (!World)
    {
        UE_LOG(LogTemp, Warning, TEXT("NULL : GetWorld"));
        return;
    }

    FString NetModeString;
    switch (World->GetNetMode())
    {
    case NM_Standalone:
        NetModeString = TEXT("Standalone");
        break;
    case NM_ListenServer:
        NetModeString = TEXT("Listen Server");
        break;
    case NM_DedicatedServer:
        NetModeString = TEXT("Dedicated Server");
        break;
    case NM_Client:
        NetModeString = TEXT("Client");
        break;
    default:
        NetModeString = TEXT("Exception : GetNetMode");
        break;
    }

    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, FString::Printf(TEXT("NetModeString : %s"), *NetModeString));
    }

    UE_LOG(LogTemp, Log, TEXT("NetModeString : %s"), *NetModeString);
}
