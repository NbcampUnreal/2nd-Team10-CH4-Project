#include "Framework/SFRoomPlayerController.h"
#include "Framework/SFGameInstanceSubsystem.h"
#include "Framework/SFGameStateBase.h"
#include "Framework/SFCheatManager.h"
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