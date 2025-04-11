#pragma once

#include "CoreMinimal.h"
#include "UI/BaseUserWidget.h"
#include "UI/PopUp/SelectPopUpBase.h"
#include "ShopItemListMenu.generated.h"

class UTextBlock;
class UButton;
class ULobbyMenu;
class UShopMenu;

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

	void OnExitClicked() override;
	
};
