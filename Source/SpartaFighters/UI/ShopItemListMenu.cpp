#include "UI/ShopItemListMenu.h"
#include "ShopMenu.h"
#include "LobbyMenu.h"

void UShopItemListMenu::NativeConstruct()
{
}

void UShopItemListMenu::OnExitClicked()
{
    UE_LOG(LogTemp, Warning, TEXT("OnCosmeticItemClicked!"));
    // 1. Hide this menu
    this->SetVisibility(ESlateVisibility::Hidden);

    // 2. 로비 메뉴 생성
    ULobbyMenu* LobbyMenuWidget = ParentShopMenu->GetLobbyMenuWidget();
    LobbyMenuWidget->SetVisibility(ESlateVisibility::Visible);
}