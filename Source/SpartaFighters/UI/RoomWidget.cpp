#include "UI/RoomWidget.h"
#include "Components/TextBlock.h"
#include "Components/ScrollBox.h"
#include "Components/EditableTextBox.h"
#include "Components/Button.h"

void URoomWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (LobbyButton)
	{
		LobbyButton->OnClicked.AddDynamic(this, &URoomWidget::OnLobbyButtonClicked);
	}

	if (ChatInputBox)
	{
		ChatInputBox->OnTextCommitted.AddDynamic(this, &URoomWidget::OnChatMessageEntered);
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
	if (!PlayerListBox) 
	{
		return;
	}

	PlayerListBox->ClearChildren();

	for (int32 i = 0; i < CurrentRoomInfo.CurrentPlayers; i++)
	{
		UTextBlock* PlayerEntry = NewObject<UTextBlock>(this);
		if (PlayerEntry)
		{
			PlayerEntry->SetText(FText::FromString(FString::Printf(TEXT("Player %d"), i + 1)));
			PlayerListBox->AddChild(PlayerEntry);
		}
	}
}

void URoomWidget::OnLobbyButtonClicked()
{
	UE_LOG(LogTemp, Log, TEXT("Leaving Room: %s"), *CurrentRoomInfo.RoomName);
}

void URoomWidget::OnChatMessageEntered(const FText& Text, ETextCommit::Type CommitMethod)
{
	if (CommitMethod == ETextCommit::OnEnter && !Text.IsEmpty())
	{
		if (ChatBox)
		{
			UTextBlock* ChatMessage = NewObject<UTextBlock>(this);
			if (ChatMessage)
			{
				ChatMessage->SetText(Text);
				ChatBox->AddChild(ChatMessage);
			}
		}
		ChatInputBox->SetText(FText::GetEmpty());
	}
}
