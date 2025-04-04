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
class UCreateRoomWidget;
class UPlayerSimpleInfoWidget;
class UGlobalChatWidget;
class URoomListWidget;

UCLASS()
class SPARTAFIGHTERS_API ULobbyMenu : public UUserWidget
{
	GENERATED_BODY()
	
protected:
		virtual void NativeConstruct() override;

private:
	// Player Info Section
	UPROPERTY(meta = (BindWidget))
	UPlayerSimpleInfoWidget* PlayerSimpleInfoWidgetClass;
	UPROPERTY(meta = (BindWidget))
	UButton* PlayerInfoButton;
	// Utility Section
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
	URoomListWidget* RoomListWidgetClass;
	// Chat Section
	UPROPERTY(meta = (BindWidget))
	UGlobalChatWidget* GlobalChatWidgetClass;
	// User List Section
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

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSoftClassPtr<UQuitGameWidget> QuitGameWidgetClass;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSoftClassPtr<UCreateRoomWidget> CreateRoomWidgetClass;
};
