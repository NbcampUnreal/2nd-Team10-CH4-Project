#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DataTypes/RoomInfo.h"
#include "RoomWidget.generated.h"

class UTextBlock;
class UScrollBox;
class UEditableTextBox;
class UButton;

UCLASS()
class SPARTAFIGHTERS_API URoomWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void SetupRoom(const FRoomInfo& RoomInfo);

protected:
	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* RoomNameText;
	UPROPERTY(meta = (BindWidget))
	UScrollBox* PlayerListBox;
	UPROPERTY(meta = (BindWidget))
	UScrollBox* ChatBox;
	UPROPERTY(meta = (BindWidget))
	UEditableTextBox* ChatInputBox;
	UPROPERTY(meta = (BindWidget))
	UButton* LobbyButton;

	UFUNCTION()
	void OnLobbyButtonClicked();
	UFUNCTION()
	void OnChatMessageEntered(const FText& Text, ETextCommit::Type CommitMethod);

private:
	FRoomInfo CurrentRoomInfo;
	void UpdatePlayerList();
};
