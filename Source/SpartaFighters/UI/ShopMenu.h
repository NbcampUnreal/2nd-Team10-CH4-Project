

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ShopMenu.generated.h"

class UTextBlock;
class UButton;
class UQuitToLobbyWidget;

UCLASS()
class SPARTAFIGHTERS_API UShopMenu : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

private:
	UPROPERTY(meta = (BindWidget))
	UButton* CommonItemButton;
	UPROPERTY(meta = (BindWidget))
	UButton* ExclusiveItemButton;
	UPROPERTY(meta = (BindWidget))
	UButton* CosmeticItemButton;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* CurrentGoldTextBlock;
	UPROPERTY(meta=(BindWidget))
	UButton* ExitButton;

	UFUNCTION()
	void OnCommonItemClicked();
	UFUNCTION()
	void OnExclusiveItemClicked();
	UFUNCTION()
	void OnCosmeticItemClicked();

public:
	/*UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UQuitToLobbyWidget> QuitToLobbyWidgetClass;
	UPROPERTY()
	UQuitToLobbyWidget* QuitToLobbyWidget;*/

};
