#pragma once

#include "CoreMinimal.h"
#include "UI/BaseUserWidget.h"
#include "UI/PopUp/SelectPopUpBase.h"
#include "Items/FSFItemData.h"
#include "Items/SFItemBase.h"
#include "ShopItemListMenu.generated.h"


class UTextBlock;
class UButton;
class ULobbyMenu;
class UShopMenu;
class UShopItemWidget;
class UListView;
class USFInventoryComponent;
class USFGameInstanceSubsystem;

UCLASS()
class SPARTAFIGHTERS_API UShopItemListMenu : public USelectPopUpBase
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

private:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* ShopItemTypeTextBlock;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* CurrentGoldTextBlock;
	UPROPERTY(meta = (BindWidget))
	UListView* ItemListView;
	UPROPERTY(meta = (BindWidget))
	UButton* PurchaseButton;
	UPROPERTY(meta = (BindWidget))
	UButton* EquipButton;
	UPROPERTY()
	TArray<USFItemBase*> ShopItems;
	UPROPERTY()
	USFItemBase* SelectedItem;

	UPROPERTY(EditDefaultsOnly, Category = "Shop Data")
	UDataTable* ItemDataTable;

public:
	UFUNCTION(BlueprintCallable)
	float GetCurrentGold();
	UFUNCTION(BlueprintCallable)
	void PurchaseSelectedItem();
	//add item to list
	UFUNCTION(BlueprintCallable)
	void PopulateItemList();
	UFUNCTION(BlueprintCallable)
	void OnItemSelectionChanged(UObject* SelectedItemObject);
	UFUNCTION(BlueprintCallable)
	void OnPurchaseButtonClicked();
	UFUNCTION(BlueprintCallable)
	void OnEquipButtonClicked();
	void UpdateCurrentGoldText();
	void OnExitClicked() override;
	
	void SetItemTypeTextBlock(EItemType Type);
	void UpdateEquipButtonVisibility();

	USFInventoryComponent* GetInventoryComponent();
	USFGameInstanceSubsystem* GetGameInstanceSubsystem();
};
