#include "UIManager.h"
#include "UIManagerSettings.h"
#include "UI/UIElements/LobbyMenu.h"
#include "UI/UIElements/ShopMenu.h"
#include "UI/UIElements/LoginMenu.h"
#include "UI/UIElements/ShopItemListMenu.h"
#include "Framework/SFGameInstance.h"

void UUIManager::Init(APlayerController* PlayerController)
{
	OwningPlayer = PlayerController;

	if (const USFGameInstance* GameInstance = Cast<USFGameInstance>(PlayerController->GetGameInstance()))
	{
		if (const UUIManagerSettings* Settings = GameInstance->GetUISettings())
		{
			LobbyMenuClass = Settings->FromBPLobbyMenuClass;
			ShopMenuClass = Settings->FromBPShopMenuClass;
			LoginMenuClass = Settings->FromBPLoginMenuClass;
			ShopItemListMenuClass = Settings->FromBPShopItemListMenuClass;

			if (LoginMenuClass)
			{
				CachedLoginMenu = CreateWidget<ULoginMenu>(OwningPlayer, LoginMenuClass);
				CachedLoginMenu->AddToViewport();
			}

			if (LobbyMenuClass)
			{
				CachedLobbyMenu = CreateWidget<ULobbyMenu>(OwningPlayer, LobbyMenuClass);
				CachedLobbyMenu->AddToViewport();
				CachedLobbyMenu->SetVisibility(ESlateVisibility::Hidden);
			}

			if (ShopMenuClass)
			{
				CachedShopMenu = CreateWidget<UShopMenu>(OwningPlayer, ShopMenuClass);
				CachedShopMenu->AddToViewport();
				CachedShopMenu->SetVisibility(ESlateVisibility::Hidden);
			}

			if (ShopItemListMenuClass)
			{
				CachedShopItemListMenu = CreateWidget<UShopItemListMenu>(OwningPlayer, ShopItemListMenuClass);
				CachedShopItemListMenu->AddToViewport();
				CachedShopItemListMenu->SetVisibility(ESlateVisibility::Hidden);
			}
		}
	}
}

void UUIManager::ShowLobbyMenu()
{
	CachedLobbyMenu->SetVisibility(ESlateVisibility::Visible);
}
	
void UUIManager::ShowShopMenu()
{
	CachedLobbyMenu->SetVisibility(ESlateVisibility::Hidden);
	CachedShopMenu->SetVisibility(ESlateVisibility::Visible);
}

void UUIManager::ShowShopItemListMenu()
{
	CachedShopMenu->SetVisibility(ESlateVisibility::Hidden);
	CachedShopItemListMenu->SetVisibility(ESlateVisibility::Visible);
}

void UUIManager::BackToLobbyMenu()
{
	CachedShopMenu->SetVisibility(ESlateVisibility::Hidden);
	CachedShopItemListMenu->SetVisibility(ESlateVisibility::Hidden);
	CachedLobbyMenu->SetVisibility(ESlateVisibility::Visible);
}
