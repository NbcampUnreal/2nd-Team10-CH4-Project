#include "Framework/SFLobbyPlayerController.h"
#include "Framework/SFGameInstanceSubsystem.h"
#include "Framework/SFCheatManager.h"

#include "UI/UIManager/UIManager.h"

#include "Net/UnrealNetwork.h"

ASFLobbyPlayerController::ASFLobbyPlayerController()
{
	CheatClass = USFCheatManager::StaticClass();
}

void ASFLobbyPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (IsLocalController())
	{
		if (USFGameInstanceSubsystem* Subsystem = GetGameInstance()->GetSubsystem<USFGameInstanceSubsystem>())
		{
			if (UUIManager* UIManager = Subsystem->GetUIManager())
			{
				UIManager->ShowLobbyMenu();
			}
		}
	}
}
