#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "DataTypes/RoomInfo.h"
#include "UIManager.generated.h"

class ULoginMenu;
class ULobbyMenu;
class UShopMenu;
class ULoginMenu;
class UUIManagerSettings;
class UShopItemListMenu;
class URoomWidget;

UCLASS()
class SPARTAFIGHTERS_API UUIManager : public UObject
{
	GENERATED_BODY()

public:
    void Init(APlayerController* PlayerController);

    UPROPERTY(VisibleAnywhere, Category = "UI")
    UUserWidget* CurrentWidget;

    UUIManager();

    /* Menu Change */
    void ShowLoginMenu();
    void ShowLobbyMenu();
    void ShowRoomMenu();

    void ShowShopMenu();
    void ShowShopItemListMenu();
    void SwitchToWidget(UUserWidget* NewWidget);
    void BackToLobbyMenu();
    void ShowCreatedRoomUI(const FRoomInfo& RoomInfo);

    void OpenRoomWidget(const FRoomInfo& RoomInfo);

    /** Get Widget */
    ULobbyMenu* GetCachedLobbyMenu() const { return CachedLobbyMenu; }
    UShopMenu* GetCachedShopMenu() const { return CachedShopMenu; }

private:
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
