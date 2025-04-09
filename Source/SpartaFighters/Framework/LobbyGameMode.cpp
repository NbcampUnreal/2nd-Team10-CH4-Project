#include "Framework/LobbyGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Framework/SFGameInstanceSubsystem.h"
#include "UI/UIManager/UIManager.h"

void ALobbyGameMode::BeginPlay()
{
    Super::BeginPlay();

    if (IsRunningDedicatedServer())
    {
        return;
    }

    if (USFGameInstanceSubsystem* Subsystem = GetGameInstance()->GetSubsystem<USFGameInstanceSubsystem>())
    {
        if (UUIManager* UIManager = Subsystem->GetUIManager())
        {
            UIManager->ShowLobbyMenu();
        }
    }
}