#include "Framework/SFRoomPlayerController.h"
#include "Framework/SFGameInstanceSubsystem.h"
#include "Framework/SFGameInstance.h"
#include "Framework/SFGameStateBase.h"
#include "Framework/SFCheatManager.h"
#include "Framework/SFPlayerState.h"
#include "UI/UIManager/UIManager.h"

ASFRoomPlayerController::ASFRoomPlayerController()
{
    CheatClass = USFCheatManager::StaticClass();
}

void ASFRoomPlayerController::BeginPlay()
{
    Super::BeginPlay();

    if (IsLocalController())
    {
        if (USFGameInstanceSubsystem* Subsystem = GetGameInstance()->GetSubsystem<USFGameInstanceSubsystem>())
        {
            if (UUIManager* UIManager = Subsystem->GetUIManager())
            {
                UIManager->ShowRoomMenu();
            }
        }
    }
}

void ASFRoomPlayerController::Server_SetReady_Implementation(bool bReady)
{
    ASFPlayerState* PS = GetPlayerState<ASFPlayerState>();
    if(PS)
    {
        PS->bIsReady = bReady;
    }
}

void ASFRoomPlayerController::Server_RequestLevelChangeByMapName_Implementation(const FString& MapName)
{
    if (USFGameInstanceSubsystem* Subsystem = GetGameInstance()->GetSubsystem<USFGameInstanceSubsystem>())
    {
        Subsystem->ChangeLevelByMapName(MapName); 
    }
}