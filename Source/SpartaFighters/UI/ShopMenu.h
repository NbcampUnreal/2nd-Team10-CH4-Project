#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ShopMenu.generated.h"

class UTextBlock;
class UButton;
class ULobbyMenu;
class UShopItemListMenu;

UCLASS()
class SPARTAFIGHTERS_API UShopMenu : public UUserWidget
{
	GENERATED_BODY()
	
public:
	ULobbyMenu* GetLobbyMenuWidget() const;

protected:
	virtual void NativeConstruct() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<ULobbyMenu> LobbyMenuClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UShopItemListMenu> ShopItemListMenuClass;

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

	UShopItemListMenu* ShopItemListMenuWidget;
	ULobbyMenu* LobbyMenuWidget;
	

	UFUNCTION()
	void OnCommonItemClicked();
	UFUNCTION()
	void OnExclusiveItemClicked();
	UFUNCTION()
	void OnCosmeticItemClicked();
	UFUNCTION()
	void OnExitClicked();

};
