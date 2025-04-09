#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DataTypes/RoomInfo.h"
#include "DataTypes/PlayerInfo.h"
#include "RoomWidget.generated.h"

class UTextBlock;
class URoomChatWidget;
class UUniformGridPanel;
class UButton;
class UPlayerSimpleInfoWidget;
class UMapSelectionWidget;
class UPlayerSlotWidget;

UCLASS()
class SPARTAFIGHTERS_API URoomWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void SetupRoom(const FRoomInfo& RoomInfo);

protected:
	virtual void NativeConstruct() override;

private:
	UPROPERTY(meta = (BindWidget))
	TSoftObjectPtr<UPlayerSimpleInfoWidget> PlayerSimpleInfoWidgetClass;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* RoomNameText;
	UPROPERTY(meta = (BindWidget))
	TSoftObjectPtr<URoomChatWidget> RoomChatWidgetClass;
	UPROPERTY(meta = (BindWidget))
	UButton* LobbyButton;
	UPROPERTY(meta = (BindWidget))
	UButton* ReadyOrStartButton;

	UPROPERTY(meta = (BindWidget))
	UUniformGridPanel* PlayerGridPanel;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UPlayerSlotWidget> PlayerSlotWidgetClass;
	TArray<UPlayerSlotWidget*> PlayerSlots;

	UPROPERTY()
	FRoomInfo CurrentRoomInfo;

	UPROPERTY()
	TArray<FPlayerInfo> PlayerList;

	const int32 MaxPlayers = 4;
	const int32 NumColumns = 2;

	UFUNCTION()
	void OnLobbyButtonClicked();
	UFUNCTION()
	void OnReadyOrStartButtonClicked();

public:
	UPROPERTY(meta = (BindWidget))
	UMapSelectionWidget* MapSelectionWidgetClass;

	UFUNCTION()
	void SetPlayerList(const TArray<FPlayerInfo>& NewPlayerList);
	UFUNCTION()
	void UpdatePlayerList();

	FRoomInfo GetCurrentRoomInfo() const { return CurrentRoomInfo; }
};
