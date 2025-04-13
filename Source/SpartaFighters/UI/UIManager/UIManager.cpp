#include "UIManager.h"
#include "UIManagerSettings.h"

#include "UI/UIElements/LoginMenu.h"
#include "UI/UIElements/LobbyMenu.h"
#include "UI/UIElements/ShopMenu.h"
#include "UI/UIElements/ShopItemListMenu.h"
#include "UI/UIElements/RoomWidget.h"
#include "UI/UIElements/CombatHUD.h"
#include "UI/UIElements/CombatResultHUD.h"
#include "UI/UIObject/MapSelectionWidget.h"
#include "UI/UIObject/SelectCharacterWidget.h"

#include "Blueprint/WidgetBlueprintLibrary.h"

#include "Kismet/GameplayStatics.h"
#include "Framework/SFGameInstance.h"

UUIManager::UUIManager()
{
	CurrentWidget = nullptr;
}

void UUIManager::Init(APlayerController* PlayerController)
{
	OwningPlayer = PlayerController;

	if (const USFGameInstance* GameInstance = Cast<USFGameInstance>(PlayerController->GetGameInstance()))
	{
		if (const UUIManagerSettings* Settings = GameInstance->GetUISettings())
		{
			LoginMenuClass = Settings->FromBPLoginMenuClass;
			LobbyMenuClass = Settings->FromBPLobbyMenuClass;
			ShopMenuClass = Settings->FromBPShopMenuClass;
			ShopItemListMenuClass = Settings->FromBPShopItemListMenuClass;
			RoomWidgetClass = Settings->FromBPRoomWidgetClass;
			MapSelectionWidgetClass = Settings->FromBPMapSelectionWidgetClass;
			CombatHUDClass = Settings->FromBPCombatHUDClass;
			CombatResultHUDClass = Settings->FromBPCombatResultHUDClass;
			SelectCharacterWidgetClass = Settings->FromBPSelectCharacterWidgetClass;

			if (!CachedLoginMenu && LoginMenuClass)
			{
				CachedLoginMenu = CreateWidget<ULoginMenu>(OwningPlayer, LoginMenuClass);
			}
			if (!CachedLobbyMenu && LobbyMenuClass)
			{
				CachedLobbyMenu = CreateWidget<ULobbyMenu>(OwningPlayer, LobbyMenuClass);
			}
			if (!CachedRoomMenu && RoomWidgetClass)
			{
				CachedRoomMenu = CreateWidget<URoomWidget>(OwningPlayer, RoomWidgetClass);
			}
			if (!CachedShopMenu && ShopMenuClass)
			{
				CachedShopMenu = CreateWidget<UShopMenu>(OwningPlayer, ShopMenuClass);
			}
			if (!CachedShopItemListMenu && ShopItemListMenuClass)
			{
				CachedShopItemListMenu = CreateWidget<UShopItemListMenu>(OwningPlayer, ShopItemListMenuClass);
			}
			if (!CachedCombatHUD && CombatHUDClass)
			{
				CachedCombatHUD = CreateWidget<UCombatHUD>(OwningPlayer, CombatHUDClass);
				CachedCombatHUD->AddToViewport();
			}
			if (!CachedCombatResultHUD && CombatResultHUDClass)
			{
				CachedCombatResultHUD = CreateWidget<UCombatResultHUD>(OwningPlayer, CombatResultHUDClass);
				CachedCombatResultHUD->AddToViewport();
			}
		}
	}
}

void UUIManager::ShowLoginMenu()
{
	UE_LOG(LogTemp, Warning, TEXT("ShowLoginMenu"));
	SwitchToWidget(CachedLoginMenu);
}

void UUIManager::ShowLobbyMenu()
{
	UE_LOG(LogTemp, Warning, TEXT("ShowLobbyMenu"));
	SwitchToWidget(CachedLobbyMenu);
}

void UUIManager::ShowRoomMenu()
{
	UE_LOG(LogTemp, Warning, TEXT("ShowRoomMenu"));
	SwitchToWidget(CachedRoomMenu);
}

void UUIManager::ShowShopMenu()
{
	UE_LOG(LogTemp, Warning, TEXT("ShowShopMenu"));
	SwitchToWidget(CachedShopMenu);
}

void UUIManager::ShowShopItemListMenu()
{
	UE_LOG(LogTemp, Warning, TEXT("ShowShopItemListMenu"));
	SwitchToWidget(CachedShopItemListMenu);
}

void UUIManager::SwitchToWidget(UUserWidget* NewWidget)
{
	if (!NewWidget || IsRunningDedicatedServer())
	{
		return;
	}
	
	if (CurrentWidget && CurrentWidget != NewWidget)
	{
		if (CurrentWidget->IsInViewport())
		{
			CurrentWidget->RemoveFromParent();
		}
	}

	if (!NewWidget->IsInViewport())
	{
		NewWidget->AddToViewport();
	}
	else
	{
		NewWidget->SetVisibility(ESlateVisibility::Visible);
	}

	CurrentWidget = NewWidget;

	if (OwningPlayer)
	{
		FInputModeUIOnly InputMode;
		InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
		OwningPlayer->SetInputMode(InputMode);
		OwningPlayer->bShowMouseCursor = true;
	}
}

void UUIManager::ShowMapSelectionWidget(EGameModeType GameModeType)
{
	if (!MapSelectionWidgetInstance && MapSelectionWidgetClass)
	{
		MapSelectionWidgetInstance = CreateWidget<UMapSelectionWidget>(GetWorld(), MapSelectionWidgetClass);
		MapSelectionWidgetInstance->AddToViewport();
	}

	if (MapSelectionWidgetInstance)
	{
		MapSelectionWidgetInstance->SetGameMode(GameModeType);
		MapSelectionWidgetInstance->SetVisibility(ESlateVisibility::Visible);
	}
}

void UUIManager::ShowSelectCharacterWidget()
{
	if (!SelectCharacterWidgetInstance && SelectCharacterWidgetClass)
	{
		SelectCharacterWidgetInstance = CreateWidget<USelectCharacterWidget>(GetWorld(), SelectCharacterWidgetClass);
		SelectCharacterWidgetInstance->AddToViewport();
	}

	if (SelectCharacterWidgetInstance)
	{
		SelectCharacterWidgetInstance->SetVisibility(ESlateVisibility::Visible);
	}
}
}

void UUIManager::ShowCombatHUD()
{
	ensureAlways(CachedCombatHUD);
	CachedCombatHUD->SetVisibility(ESlateVisibility::Visible);
}

void UUIManager::ShowCombatResultHUD()
{
	ensureAlways(CachedCombatResultHUD);
	CachedCombatResultHUD->SetVisibility(ESlateVisibility::Visible);
}

void UUIManager::CloseCombatHUD()
{
	ensureAlways(CachedCombatHUD);
	CachedCombatHUD->SetVisibility(ESlateVisibility::Hidden);
}

void UUIManager::CloseCombatResultHUD()
{
	ensureAlways(CachedCombatResultHUD);
	CachedCombatResultHUD->SetVisibility(ESlateVisibility::Hidden);
}
