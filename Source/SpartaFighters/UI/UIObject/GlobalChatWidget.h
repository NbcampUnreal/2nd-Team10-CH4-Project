#pragma once

#include "CoreMinimal.h"
#include "UI/UIObject/ChatBoxWidgetBase.h"
#include "GlobalChatWidget.generated.h"

UCLASS()
class SPARTAFIGHTERS_API UGlobalChatWidget : public UChatBoxWidgetBase
{
	GENERATED_BODY()
	
public:
	void ReceiveGlobalMessage(const FString& PlayerName, const FString& Message);
};
