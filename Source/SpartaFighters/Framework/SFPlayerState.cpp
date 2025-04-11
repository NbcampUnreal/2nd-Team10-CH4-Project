#include "Framework/SFPlayerState.h"
#include "Framework/SFGameInstanceSubsystem.h"
#include "UI/UIManager/UIManager.h"
#include "UI/UIElements/RoomWidget.h"
#include "Net/UnrealNetwork.h"

ASFPlayerState::ASFPlayerState()
{
	bReplicates = true;
}

void ASFPlayerState::Server_SetPlayerID_Implementation(const FString& InPlayerID)
{
	
			UE_LOG(LogTemp, Warning, TEXT("[SFPlayerState->PlayerUniqueID : %s]"), *PlayerUniqueID);
		
	if (HasAuthority())
	{
		PlayerUniqueID = InPlayerID;
		UE_LOG(LogTemp, Warning, TEXT("[SFPlayerState->PlayerUniqueID : %s]"), *PlayerUniqueID);

	}
}

void ASFPlayerState::OnRep_PlayerUniqueID()
{
	UE_LOG(LogTemp, Warning, TEXT("OnRep_PlayerUniqueID Called"));


	UE_LOG(LogTemp, Warning, TEXT("OnRep_PlayerID triggered on client: %s"), *PlayerUniqueID);

	if (const USFGameInstanceSubsystem* Subsystem = GetGameInstance()->GetSubsystem<USFGameInstanceSubsystem>())
	{
		if (UUIManager* UIManager = Subsystem->GetUIManager())
		{
			if (URoomWidget* RoomWidget = Cast<URoomWidget>(UIManager->GetCurrentWidget()))
			{
				RoomWidget->UpdatePlayerSlots();
			}
		}
	}
}

void ASFPlayerState::AddDeathCount()
{
	if (HasAuthority())
	{
		++DeathCount;
		UE_LOG(LogTemp, Log, TEXT("%s died. DeathCount = %d"), *GetPlayerName(), DeathCount);
	}
}

void ASFPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ASFPlayerState, PlayerUniqueID);
	DOREPLIFETIME(ASFPlayerState, SelectedCharacterName);
	DOREPLIFETIME(ASFPlayerState, CharacterTexturePath);
	DOREPLIFETIME(ASFPlayerState, EquippedItems);
	DOREPLIFETIME(ASFPlayerState, DeathCount);
}