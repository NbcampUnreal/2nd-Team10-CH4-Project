#include "Framework/SFCheatManager.h"
#include "Framework/SFGameInstance.h"
#include "Framework/SFGameInstanceSubsystem.h"

#include "GameFramework/GameModeBase.h"
#include "GameFramework/GameStateBase.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/PlayerState.h"
#include "GameFramework/Pawn.h"

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

void USFCheatManager::PrintFrameworkInfo()
{
    UWorld* World = GetWorld();
    if (!World)
    {
        UE_LOG(LogTemp, Warning, TEXT("World is null"));
        return;
    }

    // GameMode
    AGameModeBase* GameMode = World->GetAuthGameMode();

    // GameState
    AGameStateBase* GameState = World->GetGameState();

    // PlayerController
    APlayerController* PC = World->GetFirstPlayerController();

    // PlayerState
    APlayerState* PlayerState = PC ? PC->PlayerState : nullptr;

    // Pawn
    APawn* Pawn = PC ? PC->GetPawn() : nullptr;

    auto LogAndScreen = [](const FString& Label, const UObject* Object)
        {
            FString Msg = FString::Printf(TEXT("%s: %s"), *Label, Object ? *Object->GetName() : TEXT("None"));
            UE_LOG(LogTemp, Log, TEXT("%s"), *Msg);
            if (GEngine)
            {
                GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, Msg);
            }
        };

    LogAndScreen(TEXT("GameMode"), GameMode);
    LogAndScreen(TEXT("GameState"), GameState);
    LogAndScreen(TEXT("PlayerController"), PC);
    LogAndScreen(TEXT("PlayerState"), PlayerState);
    LogAndScreen(TEXT("Pawn"), Pawn);
}