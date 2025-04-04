#include "UI/ShopMenu.h"
#include "UI/LobbyMenu.h"
#include "UI/ShopItemListMenu.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"

void UShopMenu::NativeConstruct()
{
    Super::NativeConstruct();

    if (CommonItemButton)
    {
        CommonItemButton->OnClicked.AddDynamic(this, &UShopMenu::OnCommonItemClicked);
    }
    if (ExclusiveItemButton)
    {
        ExclusiveItemButton->OnClicked.AddDynamic(this, &UShopMenu::OnExclusiveItemClicked);
    }
    if (CosmeticItemButton)
    {
        CosmeticItemButton->OnClicked.AddDynamic(this, &UShopMenu::OnCosmeticItemClicked);
    }
    if (ExitButton)
    {
        ExitButton->OnClicked.AddDynamic(this, &UShopMenu::OnExitClicked);
    }

    if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
    {
        FInputModeUIOnly InputModeData;
        PlayerController->SetInputMode(InputModeData);
        PlayerController->bShowMouseCursor = true;

        ShopItemListMenuWidget = CreateWidget<UShopItemListMenu>(PlayerController, ShopItemListMenuClass);
        ShopItemListMenuWidget->AddToViewport();
        LobbyMenuWidget = CreateWidget<ULobbyMenu>(PlayerController, LobbyMenuClass);
        LobbyMenuWidget->AddToViewport();
        LobbyMenuWidget->SetVisibility(ESlateVisibility::Hidden);
    }
}

ULobbyMenu* UShopMenu::GetLobbyMenuWidget() const
{
    return LobbyMenuWidget;
}

void UShopMenu::OnCommonItemClicked()
{
    UE_LOG(LogTemp, Warning, TEXT("OnCommonItemClicked"));
    this->SetVisibility(ESlateVisibility::Hidden);
    ShopItemListMenuWidget->SetVisibility(ESlateVisibility::Visible);
    
}

void UShopMenu::OnExclusiveItemClicked()
{
    UE_LOG(LogTemp, Warning, TEXT("OnExclusiveItemClicked!"));
    this->SetVisibility(ESlateVisibility::Hidden);
    ShopItemListMenuWidget->SetVisibility(ESlateVisibility::Visible);
    
}

void UShopMenu::OnCosmeticItemClicked()
{
    UE_LOG(LogTemp, Warning, TEXT("OnCosmeticItemClicked!"));
    this->SetVisibility(ESlateVisibility::Hidden);
    ShopItemListMenuWidget->SetVisibility(ESlateVisibility::Visible);
   
}

void UShopMenu::OnExitClicked()
{
    UE_LOG(LogTemp, Warning, TEXT("OnExitClicked!"));
    // 1. Hide Shop Menu
    this->SetVisibility(ESlateVisibility::Hidden);
    // 2. Set Visible Lobby Menu
    LobbyMenuWidget->SetVisibility(ESlateVisibility::Visible);
}
