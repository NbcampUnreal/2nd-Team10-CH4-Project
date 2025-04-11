#include "ShopItemListMenu.h"
#include "Components/Button.h"
#include "UI/UIManager/UIManager.h"

void UShopItemListMenu::NativeConstruct()
{
    Super::NativeConstruct();
}

void UShopItemListMenu::NativeDestruct()
{
    Super::NativeDestruct();
}

void UShopItemListMenu::OnExitClicked()
{
    if (UUIManager* UIManager = ResolveUIManager())
    {
        UIManager->ShowLobbyMenu();
    }
}