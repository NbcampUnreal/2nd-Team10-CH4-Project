#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "UIManagerSettings.generated.h"

class ULoginMenu;
class ULobbyMenu;
class URoomWidget;
class UShopMenu;
class UShopItemListMenu;
class UMapSelectionWidget;
class USelectCharacterWidget;
class UCombatHUD;
class UCombatResultHUD;

UCLASS(BlueprintType)
class SPARTAFIGHTERS_API UUIManagerSettings : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<ULoginMenu> FromBPLoginMenuClass;
	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<ULobbyMenu> FromBPLobbyMenuClass;
	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<URoomWidget> FromBPRoomWidgetClass;
	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UShopMenu> FromBPShopMenuClass;
	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UShopItemListMenu> FromBPShopItemListMenuClass;
	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UMapSelectionWidget> FromBPMapSelectionWidgetClass;
	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<USelectCharacterWidget> FromBPSelectCharacterWidgetClass;
	TSubclassOf<UCombatHUD> FromBPCombatHUDClass;
	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UCombatResultHUD> FromBPCombatResultHUDClass;
	
};
