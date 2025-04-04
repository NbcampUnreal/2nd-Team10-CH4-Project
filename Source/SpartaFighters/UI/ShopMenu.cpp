#include "UI/ShopMenu.h"
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

    if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
    {
        FInputModeUIOnly InputModeData;
        PlayerController->SetInputMode(InputModeData);
        PlayerController->bShowMouseCursor = true;
    }
}

void UShopMenu::OnCommonItemClicked()
{
    UE_LOG(LogTemp, Warning, TEXT("OnCommonItemClicked"));
    
}

void UShopMenu::OnExclusiveItemClicked()
{
    UE_LOG(LogTemp, Warning, TEXT("OnExclusiveItemClicked!"));
    
}

void UShopMenu::OnCosmeticItemClicked()
{
    UE_LOG(LogTemp, Warning, TEXT("OnCosmeticItemClicked!"));
   
}
