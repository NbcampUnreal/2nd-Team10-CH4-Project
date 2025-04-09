#include "ShopMenu.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "UI/UIManager/UIManager.h"

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
    }
}

void UShopMenu::OnCommonItemClicked()
{
    if (UUIManager* UIManager = ResolveUIManager())
	{
		UIManager->ShowShopItemListMenu();
        UE_LOG(LogTemp, Warning, TEXT("OnCommonItemClicked"));
	}
}

void UShopMenu::OnExclusiveItemClicked()
{
    if (UUIManager* UIManager = ResolveUIManager())
    {
        UIManager->ShowShopItemListMenu();
        UE_LOG(LogTemp, Warning, TEXT("OnExclusiveItemClicked!"));
    }
}

void UShopMenu::OnCosmeticItemClicked()
{
    if (UUIManager* UIManager = ResolveUIManager())
    {
        UIManager->ShowShopItemListMenu();
        UE_LOG(LogTemp, Warning, TEXT("OnCosmeticItemClicked!"));
    }
}

void UShopMenu::OnExitClicked()
{
    if (UUIManager* UIManager = ResolveUIManager())
    {
        UIManager->BackToLobbyMenu();
        UE_LOG(LogTemp, Warning, TEXT("OnExitClicked!"));
    }
}
