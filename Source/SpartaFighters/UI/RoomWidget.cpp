#include "UI/RoomWidget.h"
#include "Components/TextBlock.h"
#include "Components/ScrollBox.h"
#include "Components/EditableTextBox.h"
#include "Components/Button.h"
#include "UI/UIObject/RoomChatWidget.h"

void URoomWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (LobbyButton)
	{
		LobbyButton->OnClicked.AddDynamic(this, &URoomWidget::OnLobbyButtonClicked);
	}
}

void URoomWidget::SetupRoom(const FRoomInfo& RoomInfo)
{
	CurrentRoomInfo = RoomInfo;

	if (RoomNameText)
	{
		RoomNameText->SetText(FText::FromString(RoomInfo.RoomName));
	}

	UpdatePlayerList();
}

void URoomWidget::UpdatePlayerList()
{
	// TODO: Update the player list UI with the current room information
}

void URoomWidget::OnLobbyButtonClicked()
{
	UE_LOG(LogTemp, Log, TEXT("Leaving Room: %s"), *CurrentRoomInfo.RoomName);
}
