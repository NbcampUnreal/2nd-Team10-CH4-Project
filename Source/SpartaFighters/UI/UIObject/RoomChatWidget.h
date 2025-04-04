#pragma once

#include "CoreMinimal.h"
#include "UI/UIObject/ChatBoxWidgetBase.h"
#include "RoomChatWidget.generated.h"

UCLASS()
class SPARTAFIGHTERS_API URoomChatWidget : public UChatBoxWidgetBase
{
	GENERATED_BODY()
	
public:
	void ReceiveRoomMessage(const FString& PlayerName, const FString& Message);
};
