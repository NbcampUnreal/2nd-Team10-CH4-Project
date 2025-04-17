#include "ShopMenu.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "UI/UIManager/UIManager.h"
#include "Framework/SFPlayerState.h"
#include "Character/SFCharacter.h"
#include "Components/TextBlock.h"


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
    UpdateCurrentGoldText();
}

void UShopMenu::NativeDestruct()
{
    Super::NativeDestruct();

    if (CommonItemButton)
    {
        CommonItemButton->OnClicked.RemoveDynamic(this, &UShopMenu::OnCommonItemClicked);
    }
    if (ExclusiveItemButton)
    {
        ExclusiveItemButton->OnClicked.RemoveDynamic(this, &UShopMenu::OnExclusiveItemClicked);
    }
    if (CosmeticItemButton)
    {
        CosmeticItemButton->OnClicked.RemoveDynamic(this, &UShopMenu::OnCosmeticItemClicked);
    }
}

void UShopMenu::OnCommonItemClicked()
{
    if (UUIManager* UIManager = ResolveUIManager())
	{
		UIManager->ShowShopItemListMenu(EItemType::Common);
        UE_LOG(LogTemp, Warning, TEXT("OnCommonItemClicked"));
	}
}

void UShopMenu::OnExclusiveItemClicked()
{
    if (UUIManager* UIManager = ResolveUIManager())
    {
        UIManager->ShowShopItemListMenu(EItemType::Exclusive);
        UE_LOG(LogTemp, Warning, TEXT("OnExclusiveItemClicked!"));
    }
}

void UShopMenu::OnCosmeticItemClicked()
{
    if (UUIManager* UIManager = ResolveUIManager())
    {
        UIManager->ShowShopItemListMenu(EItemType::Cosmetic);
        UE_LOG(LogTemp, Warning, TEXT("OnCosmeticItemClicked!"));
    }
}

void UShopMenu::OnExitClicked()
{
    if (UUIManager* UIManager = ResolveUIManager())
    {
        UIManager->ShowRoomMenu();
    }
}

void UShopMenu::UpdateCurrentGoldText()
{
    if (CurrentGoldTextBlock)
    {
        CurrentGoldTextBlock->SetText(FText::AsNumber(GetCurrentGold()));
    }
}

float UShopMenu::GetCurrentGold()
{
    if (APlayerController* PlayerController = GetOwningPlayer())
    {
        if (ASFPlayerState* PlayerState = PlayerController->GetPlayerState<ASFPlayerState>())
        {
            return PlayerState->CurrentGold;
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("GetCurrentGold - PlayerState is nullptr."));
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("GetCurrentGold - PlayerController is nullptr."));
    }
    return 0.0f;
}