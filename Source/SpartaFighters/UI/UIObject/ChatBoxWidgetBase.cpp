#include "UI/UIObject/ChatBoxWidgetBase.h"
#include "Components/ScrollBox.h"
#include "Components/EditableTextBox.h"
#include "Components/TextBlock.h"

void UChatBoxWidgetBase::NativeConstruct()
{
	Super::NativeConstruct();

	if (ChatInputBox)
	{
		ChatInputBox->OnTextCommitted.AddDynamic(this, &UChatBoxWidgetBase::OnChatMessageEntered);
	}
}

void UChatBoxWidgetBase::OnChatMessageEntered(const FText& Text, ETextCommit::Type CommitMethod)
{
	if (CommitMethod == ETextCommit::OnEnter && !Text.IsEmpty())
	{
		AddChatMessage(TEXT("You"), Text.ToString());
		ChatInputBox->SetText(FText::GetEmpty());
	}
}

void UChatBoxWidgetBase::AddChatMessage(const FString& PlayerName, const FString& Message)
{
	if (!ChatBox) 
	{
		return;
	}

	UTextBlock* ChatMessage = NewObject<UTextBlock>(this);
	if (ChatMessage)
	{
		ChatMessage->SetText(FText::FromString(FString::Printf(TEXT("[%s]: %s"), *PlayerName, *Message)));
		ChatBox->AddChild(ChatMessage);
	}
}
