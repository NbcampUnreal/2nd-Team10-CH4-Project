#include "UI/UIObject/RoomChatWidget.h"

void URoomChatWidget::ReceiveRoomMessage(const FString& PlayerName, const FString& Message)
{
	AddChatMessage(PlayerName, Message);
}