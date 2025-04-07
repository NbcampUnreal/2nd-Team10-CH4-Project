#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "UIManagerSettings.generated.h"

class ULobbyMenu;
class UShopMenu;
class ULoginMenu;
class UShopItemListMenu;

UCLASS(BlueprintType)
class SPARTAFIGHTERS_API UUIManagerSettings : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, Category = "UI")
	TSoftClassPtr<ULobbyMenu> LobbyMenuClass;
	UPROPERTY(EditAnywhere, Category = "UI")
	TSoftClassPtr<UShopMenu> ShopMenuClass;
	UPROPERTY(EditAnywhere, Category = "UI")
	TSoftClassPtr<ULoginMenu> LoginMenuClass;
	UPROPERTY(EditAnywhere, Category = "UI")
	TSoftClassPtr<UShopItemListMenu> ShopItemListMenuClass;
};
