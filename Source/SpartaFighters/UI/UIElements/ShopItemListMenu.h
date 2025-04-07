#pragma once

#include "CoreMinimal.h"
#include "UI/BaseUserWidget.h"
#include "ShopItemListMenu.generated.h"

class UTextBlock;
class UButton;
class ULobbyMenu;
class UShopMenu;

UCLASS()
class SPARTAFIGHTERS_API UShopItemListMenu : public UBaseUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

private:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* ShopItemTypeTextBlock;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* CurrentGoldTextBlock;
	UPROPERTY(meta = (BindWidget))
	UButton* ExitButton;

	UFUNCTION()
	void OnExitClicked();
	
};
