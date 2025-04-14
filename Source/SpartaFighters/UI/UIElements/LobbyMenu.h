#pragma once

#include "CoreMinimal.h"
#include "UI/BaseUserWidget.h"
#include "LobbyMenu.generated.h"

class UTextBlock;
class UButton;
class UQuitGameWidget;

UCLASS()
class SPARTAFIGHTERS_API ULobbyMenu : public UBaseUserWidget
{
	GENERATED_BODY()
	
protected:
		virtual void NativeConstruct() override;
		virtual void NativeDestruct() override;

private:
	/*UPROPERTY(meta = (BindWidget))
	UButton* PlayerInfoButton;
	UPROPERTY(meta = (BindWidget))
	UButton* ShopButton;*/
	UPROPERTY(meta = (BindWidget))
	UButton* OptionButton;
	UPROPERTY(meta = (BindWidget))
	UButton* QuitGameButton;
	
	/*UPROPERTY(meta = (BindWidget))
	UButton* SingleGameModeButton;*/
	UPROPERTY(meta = (BindWidget))
	UButton* CoopGameModeButton;
	UPROPERTY(meta = (BindWidget))
	UButton* BattleGameModeButton;

	/*UFUNCTION()
	void OnSingleGameModeClicked();*/
	UFUNCTION()
	void OnCoopGameModeClicked();
	UFUNCTION()
	void OnBattleGameModeClicked();

	/*UFUNCTION()
	void OnPlayerInfoClicked();
	UFUNCTION()
	void OnShopClicked();*/
	UFUNCTION()
	void OnOptionClicked();
	UFUNCTION()
	void OnQuitGameClicked();

public:
	// TODO : Use ResolveUIManager
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSoftClassPtr<UQuitGameWidget> QuitGameWidgetClass;

	/*UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSoftClassPtr<UShopMenu> ShopMenuWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	UShopMenu* CachedShopMenuWidget;*/

	
};
