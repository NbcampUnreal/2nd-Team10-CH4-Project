#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "DataTypes/GameModeType.h"
#include "UI/UIObject/MapSelectionWidget.h"
#include "UI/UIObject/SelectCharacterWidget.h"
#include "Items/SFItemBase.h"
#include "UIManager.generated.h"

class ULoginMenu;
class ULobbyMenu;
class UShopMenu;
class UShopItemListMenu;
class UUIManagerSettings;
class URoomWidget;
class UCombatHUD;
class UCombatResultHUD;
class UOptionsWidget;

UCLASS()
class SPARTAFIGHTERS_API UUIManager : public UObject
{
	GENERATED_BODY()

public:
    UUIManager();

    void Init(APlayerController* PlayerController);
    virtual void BeginDestroy();

    void SetPlayerController(APlayerController* PlayerController);

    /* Menu Change */
    void ShowLoginMenu();
    void ShowLobbyMenu();
    void ShowRoomMenu();

    void ShowShopMenu();
    void ShowShopItemListMenu(EItemType Type);
    void SwitchToWidget(UUserWidget* NewWidget);

    /** Get Widget */
    ULobbyMenu* GetCachedLobbyMenu() const { return CachedLobbyMenu; }
    UShopMenu* GetCachedShopMenu() const { return CachedShopMenu; }
    UUserWidget* GetCurrentWidget() const { return CurrentWidget; }
    UCombatHUD* GetCachedCombatHUD() const { return CachedCombatHUD; }

    void ShowMapSelectionWidget(EGameModeType GameModeType);
    void ShowSelectCharacterWidget();

    void ShowCombatResultHUD();
    void CloseCombatHUD();
    void CloseCombatResultHUD();
    void UpdateCombatHUD();
    void DelayStartCombatResultUpdate();
    void UpdateCombatResult();

    UFUNCTION()
    void UpdateHUD();

    UFUNCTION()
    void StartHUDUpdate();
    void StartCombatResultUpdate();
    void EndCombatResultUpdate();
    void EndHUDUpdate();
    void ShowCombatHUD();

    void ShowOptionsWidget();
    void CloseOptionsWidget();

private:
    UPROPERTY()
    UMapSelectionWidget* MapSelectionWidgetInstance;
    UPROPERTY(EditDefaultsOnly)
    TSubclassOf<UMapSelectionWidget> MapSelectionWidgetClass;

    UPROPERTY()
    USelectCharacterWidget* SelectCharacterWidgetInstance;
    UPROPERTY(EditDefaultsOnly)
    TSubclassOf<USelectCharacterWidget> SelectCharacterWidgetClass;

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
    UPROPERTY(EditAnywhere, Category = "UI")
    TSubclassOf<UCombatHUD> CombatHUDClass;
    UPROPERTY(EditAnywhere, Category = "UI")
    TSubclassOf<UCombatResultHUD> CombatResultHUDClass;
    UPROPERTY(EditAnywhere, Category = "UI")
    TSubclassOf<UOptionsWidget> OptionsWidgetClass;
    
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
    UPROPERTY()
    UCombatHUD* CachedCombatHUD;
    UPROPERTY()
    UCombatResultHUD* CachedCombatResultHUD;
    UPROPERTY()
    UOptionsWidget* CachedOptionsWidget;

    /* Controller save */
    UPROPERTY()
    APlayerController* OwningPlayer;
	
    FTimerHandle HUDUpdateTimerHandle;
    FTimerHandle CombatResultHUDUpdateTimerHandle;
    FTimerHandle LazyCombatResultUpdateTimerHandle;
    FTimerHandle DelayStartCombatResultTimerHandle;
};
