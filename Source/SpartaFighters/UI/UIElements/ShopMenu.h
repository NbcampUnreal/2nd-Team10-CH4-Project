#pragma once

#include "CoreMinimal.h"
#include "UI/BaseUserWidget.h"
#include "ShopMenu.generated.h"

class UTextBlock;
class UButton;
class ULobbyMenu;
class UShopItemListMenu;

UCLASS()
class SPARTAFIGHTERS_API UShopMenu : public UBaseUserWidget
{
	GENERATED_BODY()
	
public:
	

protected:
	virtual void NativeConstruct() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSoftClassPtr<UShopItemListMenu> ShopItemListMenuClass;

private:
	UPROPERTY(meta = (BindWidget))
	UButton* CommonItemButton;
	UPROPERTY(meta = (BindWidget))
	UButton* ExclusiveItemButton;
	UPROPERTY(meta = (BindWidget))
	UButton* CosmeticItemButton;
	UPROPERTY(meta = (BindWidget))
	UButton* ExitButton;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* CurrentGoldTextBlock;

	UFUNCTION()
	void OnCommonItemClicked();
	UFUNCTION()
	void OnExclusiveItemClicked();
	UFUNCTION()
	void OnCosmeticItemClicked();
	UFUNCTION()
	void OnExitClicked();

};
