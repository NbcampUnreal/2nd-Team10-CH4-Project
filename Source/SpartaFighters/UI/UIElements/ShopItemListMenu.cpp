#include "ShopItemListMenu.h"
#include "Components/Button.h"
#include "UI/UIManager/UIManager.h"

void UShopItemListMenu::NativeConstruct()
{
    if (ExitButton)
    {
        ExitButton->OnClicked.AddDynamic(this, &UShopItemListMenu::OnExitClicked);
    }
}

void UShopItemListMenu::OnExitClicked()
{
    if (UUIManager* UIManager = ResolveUIManager())
    {
        UIManager->BackToLobbyMenu();
        UE_LOG(LogTemp, Warning, TEXT("OnExitClicked!"));
    }
}