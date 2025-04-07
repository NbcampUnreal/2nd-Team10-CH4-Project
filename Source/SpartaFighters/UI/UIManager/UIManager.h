#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "UIManager.generated.h"

class ULobbyMenu;
class UShopMenu;
class ULoginMenu;
class UUIManagerSettings;
class UShopItemListMenu;

UCLASS()
class SPARTAFIGHTERS_API UUIManager : public UObject
{
	GENERATED_BODY()

public:
    void Init(APlayerController* PlayerController);

    /* Menu Change */
    void ShowLobbyMenu();
    void ShowShopMenu();
    void ShowShopItemListMenu();
    void BackToLobbyMenu();

    /** Get Widget */
    ULobbyMenu* GetCachedLobbyMenu() const { return CachedLobbyMenu; }
    UShopMenu* GetCachedShopMenu() const { return CachedShopMenu; }

private:
    /* Soft Widget Class */
    UPROPERTY(EditDefaultsOnly, Category = "UI")
    TSoftClassPtr<ULobbyMenu> SoftLobbyMenuClass;
    UPROPERTY(EditDefaultsOnly, Category = "UI")
    TSoftClassPtr<UShopMenu> SoftShopMenuClass;
    UPROPERTY(EditDefaultsOnly, Category = "UI")
    TSoftClassPtr<ULoginMenu> SoftLoginMenuClass;
    UPROPERTY(EditDefaultsOnly, Category = "UI")
    TSoftClassPtr<UShopItemListMenu> SoftShopItemListMenuClass;

    /* Chacing Widget */
    UPROPERTY()
    ULobbyMenu* CachedLobbyMenu;
    UPROPERTY()
    UShopMenu* CachedShopMenu;
    UPROPERTY()
    ULoginMenu* CachedLoginMenu;
    UPROPERTY()
    UShopItemListMenu* CachedShopItemListMenu;

    /* Controller save */
    UPROPERTY()
    APlayerController* OwningPlayer;
	
};
