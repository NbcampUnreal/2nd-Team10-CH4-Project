#pragma once

#include "CoreMinimal.h"
#include "UI/BaseUserWidget.h"
#include "DataTypes/RoomInfo.h"
#include "DataTypes/PlayerInfo.h"
#include "RoomWidget.generated.h"

class UButton;
class UPlayerSlotWidget;

UCLASS()
class SPARTAFIGHTERS_API URoomWidget : public UBaseUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

private:
	UPROPERTY(meta = (BindWidget))
	UButton* ShopButton;
	UPROPERTY(meta = (BindWidget))
	UButton* PlayerInfoButton;
	UPROPERTY(meta = (BindWidget))
	UButton* OptionButton;
	UPROPERTY(meta = (BindWidget))
	UButton* LobbyButton;
	UPROPERTY(meta = (BindWidget))
	UButton* StartButton;
	UPROPERTY(meta = (BindWidget))
	UButton* SingleGameModeButton;
	UPROPERTY(meta = (BindWidget))
	UButton* CoopGameModeButton;
	UPROPERTY(meta = (BindWidget))
	UButton* BattleGameModeButton;

	UPROPERTY(meta = (BindWidget))
	UPlayerSlotWidget* ClientSlot1;
	UPROPERTY(meta = (BindWidget))
	UPlayerSlotWidget* ClientSlot2;
	UPROPERTY(meta = (BindWidget))
	UPlayerSlotWidget* ClientSlot3;
	UPROPERTY(meta = (BindWidget))
	UPlayerSlotWidget* ClientSlot4;

	UFUNCTION()
	void OnShopButtonClicked();
	UFUNCTION()
	void OnPlayerInfoButtonClicked();
	UFUNCTION()
	void OnOptionButtonClicked();

	UFUNCTION()
	void OnLobbyButtonClicked();
	UFUNCTION()
	void OnStartButtonClicked();
};
