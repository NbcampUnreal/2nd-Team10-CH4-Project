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
			SoftLobbyMenuClass = Settings->LobbyMenuClass;
			SoftShopMenuClass = Settings->ShopMenuClass;
			SoftLoginMenuClass = Settings->LoginMenuClass;
			SoftShopItemListMenuClass = Settings->ShopItemListMenuClass;

			if (SoftLoginMenuClass.IsNull() == false)
			{
				CachedLoginMenu = CreateWidget<ULoginMenu>(OwningPlayer, SoftLoginMenuClass.LoadSynchronous());
				CachedLoginMenu->AddToViewport();
			}

			if (SoftLobbyMenuClass.IsNull() == false)
			{
				CachedLobbyMenu = CreateWidget<ULobbyMenu>(OwningPlayer, SoftLobbyMenuClass.LoadSynchronous());
				CachedLobbyMenu->AddToViewport();
				CachedLobbyMenu->SetVisibility(ESlateVisibility::Hidden);
			}

			if (SoftShopMenuClass.IsNull() == false)
			{
				CachedShopMenu = CreateWidget<UShopMenu>(OwningPlayer, SoftShopMenuClass.LoadSynchronous());
				CachedShopMenu->AddToViewport();
				CachedShopMenu->SetVisibility(ESlateVisibility::Hidden);
			}

			if (SoftShopItemListMenuClass.IsNull() == false)
			{
				CachedShopItemListMenu = CreateWidget<UShopItemListMenu>(OwningPlayer, SoftShopItemListMenuClass.LoadSynchronous());
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
