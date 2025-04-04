#include "UI/UIObject/GlobalChatWidget.h"

void UGlobalChatWidget::ReceiveGlobalMessage(const FString& PlayerName, const FString& Message)
{
	AddChatMessage(PlayerName, Message);
}
