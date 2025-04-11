#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "DataTypes/GameModeType.h"
#include "UI/UIObject/MapSelectionWidget.h"
#include "UIManager.generated.h"

class ULoginMenu;
class ULobbyMenu;
class UShopMenu;
class UShopItemListMenu;
class UUIManagerSettings;
class URoomWidget;

UCLASS()
class SPARTAFIGHTERS_API UUIManager : public UObject
{
	GENERATED_BODY()

public:
    UUIManager();

    void Init(APlayerController* PlayerController);

    /* Menu Change */
    void ShowLoginMenu();
    void ShowLobbyMenu();
    void ShowRoomMenu();

    void ShowShopMenu();
    void ShowShopItemListMenu();
    void SwitchToWidget(UUserWidget* NewWidget);

    /** Get Widget */
    ULobbyMenu* GetCachedLobbyMenu() const { return CachedLobbyMenu; }
    UShopMenu* GetCachedShopMenu() const { return CachedShopMenu; }
    UUserWidget* GetCurrentWidget() const { return CurrentWidget; }

    void ShowMapSelectionWidget(EGameModeType GameModeType);
    void CloseMapSelectionWidget();

private:
    UPROPERTY()
    UMapSelectionWidget* MapSelectionWidgetInstance;
    UPROPERTY(EditDefaultsOnly)
    TSubclassOf<UMapSelectionWidget> MapSelectionWidgetClass;

    UPROPERTY()
    UUserWidget* CurrentWidget;

    /* Widget Class */
    UPROPERTY(EditDefaultsOnly, Category = "UI")
    TSubclassOf<ULoginMenu> LoginWidgetClass;
    UPROPERTY(EditDefaultsOnly, Category = "UI")
    TSubclassOf<ULobbyMenu> LobbyMenuClass;
    UPROPERTY(EditDefaultsOnly, Category = "UI")
    TSubclassOf<URoomWidget> RoomWidgetClass;
    UPROPERTY(EditDefaultsOnly, Category = "UI")
    TSubclassOf<UShopMenu> ShopMenuClass;
    UPROPERTY(EditDefaultsOnly, Category = "UI")
    TSubclassOf<ULoginMenu> LoginMenuClass;
    UPROPERTY(EditDefaultsOnly, Category = "UI")
    TSubclassOf<UShopItemListMenu> ShopItemListMenuClass;
    
    /* Chacing Widget */
    UPROPERTY()
    ULoginMenu* CachedLoginMenu;
    UPROPERTY()
    ULobbyMenu* CachedLobbyMenu;
    UPROPERTY()
    URoomWidget* CachedRoomMenu;
    UPROPERTY()
    UShopMenu* CachedShopMenu;
    UPROPERTY()
    UShopItemListMenu* CachedShopItemListMenu;

    /* Controller save */
    UPROPERTY()
    APlayerController* OwningPlayer;
	
};
