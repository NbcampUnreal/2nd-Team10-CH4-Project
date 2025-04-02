#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LobbyMenu.generated.h"

class UTextBlock;
class UImage;
class UButton;
class UVerticalBox;
class UEditableTextBox;
class UQuitGameWidget;

UCLASS()
class SPARTAFIGHTERS_API ULobbyMenu : public UUserWidget
{
	GENERATED_BODY()
	
protected:
		virtual void NativeConstruct() override;

private:
	// Utility Section
	UPROPERTY(meta = (BindWidget))
	UImage* PlayerIcon;
	UPROPERTY(meta = (BindWidget))
	UButton* PlayerInfoButton;
	UPROPERTY(meta = (BindWidget))
	UButton* ShopButton;
	UPROPERTY(meta = (BindWidget))
	UButton* CreateRoomButton;
	UPROPERTY(meta = (BindWidget))
	UButton* OptionButton;
	UPROPERTY(meta = (BindWidget))
	UButton* QuitGameButton;
	// Room Section
	UPROPERTY(meta = (BindWidget))
	UButton* CooperativeFilterButton;
	UPROPERTY(meta = (BindWidget))
	UButton* BattleFilterButton;
	UPROPERTY(meta = (BindWidget))
	UVerticalBox* RoomListBox;
	// Chat Section
	UPROPERTY(meta = (BindWidget))
	UVerticalBox* ChatListBox;
	UPROPERTY(meta = (BindWidget))
	UEditableTextBox* ChatTextBox;
	UPROPERTY(meta = (BindWidget))
	UVerticalBox* UserListBox;

	UFUNCTION()
	void OnPlayerInfoClicked();
	UFUNCTION()
	void OnShopClicked();
	UFUNCTION()
	void OnCreateRoomClicked();
	UFUNCTION()
	void OnOptionClicked();
	UFUNCTION()
	void OnQuitGameClicked();
	UFUNCTION()
	void OnCooperativeFilterClicked();
	UFUNCTION()
	void OnBattleFilterClicked();
	UFUNCTION()
	void OnChatTextCommitted(const FText& Text, ETextCommit::Type CommitMethod);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UQuitGameWidget> QuitGameWidgetClass;
	UPROPERTY()
	UQuitGameWidget* QuitGameWidget;
	
};
