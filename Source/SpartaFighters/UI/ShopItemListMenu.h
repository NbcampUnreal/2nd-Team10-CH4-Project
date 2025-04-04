#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ShopItemListMenu.generated.h"

class UTextBlock;
class UButton;
class ULobbyMenu;
class UShopMenu;

UCLASS()
class SPARTAFIGHTERS_API UShopItemListMenu : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<ULobbyMenu> LobbyMenuClass;

	UPROPERTY()
	UShopMenu* ParentShopMenu;

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
