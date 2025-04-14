#pragma once

#include "CoreMinimal.h"
#include "UI/BaseUserWidget.h"
#include "RoomWidget.generated.h"

class UButton;
class UPlayerSlotWidget;
class UUniformGridPanel;

UCLASS()
class SPARTAFIGHTERS_API URoomWidget : public UBaseUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

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
	UButton* SelectMapButton;
	UPROPERTY(meta = (BindWidget))
	UButton* ReadyButton;

	UPROPERTY(meta = (BindWidget))
	UUniformGridPanel* PlayerGridPanel;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UPlayerSlotWidget> PlayerSlotWidgetClass;

	FTimerHandle UpdatePlayerSlotTimerHandle;

	UFUNCTION()
	void OnShopButtonClicked();
	UFUNCTION()
	void OnPlayerInfoButtonClicked();
	UFUNCTION()
	void OnOptionButtonClicked();
	UFUNCTION()
	void OnLobbyButtonClicked();
	UFUNCTION()
	void OnSelectMapButtonClicked();
	UFUNCTION()
	void OnReadyButtonClicked();

public:
	void UpdatePlayerSlots();

	void UpdateUIState();

};
