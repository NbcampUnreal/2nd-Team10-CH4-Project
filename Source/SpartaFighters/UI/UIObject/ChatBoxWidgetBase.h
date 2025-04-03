#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ChatBoxWidgetBase.generated.h"

class UScrollBox;
class UEditableTextBox;
class UTextBlock;

UCLASS()
class SPARTAFIGHTERS_API UChatBoxWidgetBase : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void AddChatMessage(const FString& PlayerName, const FString& Message);

protected:
	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
	UScrollBox* ChatBox;

	UPROPERTY(meta = (BindWidget))
	UEditableTextBox* ChatInputBox;

	UFUNCTION()
	void OnChatMessageEntered(const FText& Text, ETextCommit::Type CommitMethod);
};
