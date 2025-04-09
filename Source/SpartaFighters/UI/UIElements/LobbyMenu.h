#pragma once

#include "CoreMinimal.h"
//#include "Blueprint/UserWidget.h"
#include "UI/BaseUserWidget.h"
#include "LobbyMenu.generated.h"

class UTextBlock;
class UButton;
class UQuitGameWidget;
// class UGlobalChatWidget;

UCLASS()
class SPARTAFIGHTERS_API ULobbyMenu : public UBaseUserWidget
{
	GENERATED_BODY()
	
protected:
		virtual void NativeConstruct() override;

private:
	UPROPERTY(meta = (BindWidget))
	UButton* PlayButton;
	UPROPERTY(meta = (BindWidget))
	UButton* PlayerInfoButton;
	UPROPERTY(meta = (BindWidget))
	UButton* ShopButton;
	UPROPERTY(meta = (BindWidget))
	UButton* OptionButton;
	UPROPERTY(meta = (BindWidget))
	UButton* QuitGameButton;
	
	//// Chat Section
	//UPROPERTY(meta = (BindWidget))
	//UGlobalChatWidget* GlobalChatWidgetClass;
	//// User List Section
	//UPROPERTY(meta = (BindWidget))
	//UVerticalBox* UserListBox;
	UFUNCTION()
	void OnPlayButtonClicked();
	UFUNCTION()
	void OnPlayerInfoClicked();
	UFUNCTION()
	void OnShopClicked();
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
