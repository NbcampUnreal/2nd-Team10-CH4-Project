#include "Framework/SFLobbyPlayerController.h"
#include "Framework/SFGameInstanceSubsystem.h"
#include "Framework/SFPlayerState.h"

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

void ASFLobbyPlayerController::Server_SelectCharacter_Implementation(FName CharacterRow, FName CommonItem, FName ExclusiveItem, FName CosmeticItem)
{
	if (ASFPlayerState* PS = GetPlayerState<ASFPlayerState>())
	{
		PS->SetSelectedCharacterRow(CharacterRow);
		//PS->SetEquippedGear(CommonItem, ExclusiveItem, CosmeticItem); 
	}
}