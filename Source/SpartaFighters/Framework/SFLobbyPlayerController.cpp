#include "Framework/SFLobbyPlayerController.h"
#include "Framework/SFGameInstanceSubsystem.h"

#include "UI/UIManager/UIManager.h"

#include "Net/UnrealNetwork.h"

ASFLobbyPlayerController::ASFLobbyPlayerController()
{
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
