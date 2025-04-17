#pragma once

#include "CoreMinimal.h"
#include "UI/PopUp/SelectPopUpBase.h"
#include "ShopMenu.generated.h"

class UTextBlock;
class UButton;
class ULobbyMenu;
class UShopItemListMenu;

UCLASS()
class SPARTAFIGHTERS_API UShopMenu : public USelectPopUpBase
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

private:
	UPROPERTY(meta = (BindWidget))
	UButton* CommonItemButton;
	UPROPERTY(meta = (BindWidget))
	UButton* ExclusiveItemButton;
	UPROPERTY(meta = (BindWidget))
	UButton* CosmeticItemButton;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* CurrentGoldTextBlock;

	UFUNCTION()
	void OnCommonItemClicked();
	UFUNCTION()
	void OnExclusiveItemClicked();
	UFUNCTION()
	void OnCosmeticItemClicked();

	void OnExitClicked() override;
	void UpdateCurrentGoldText();
	float GetCurrentGold();
};
