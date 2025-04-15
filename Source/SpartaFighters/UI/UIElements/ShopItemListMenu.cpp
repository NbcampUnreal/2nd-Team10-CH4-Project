#include "ShopItemListMenu.h"
#include "Components/Button.h"
#include "UI/UIManager/UIManager.h"
#include "Components/TextBlock.h"
#include "Components/ListView.h"
#include "UI/UIManager/UIManager.h"
#include "Items/FSFItemData.h"
#include "Framework/SFPlayerState.h"   
#include "Character/SFCharacter.h"
#include "Items/SFItemBase.h"

class UShopItemWidget;

void UShopItemListMenu::NativeConstruct()
{
    Super::NativeConstruct();

    if (PurchaseButton)
    {
        PurchaseButton->OnClicked.AddDynamic(this, &UShopItemListMenu::OnPurchaseButtonClicked);
        PurchaseButton->SetIsEnabled(false);
    }
    if (ItemListView)
    {
        ItemListView->OnItemSelectionChanged().AddUObject(this, &UShopItemListMenu::OnItemSelectionChanged);
    }
	
	if (ItemDataTable)
	{
		//Go through Item Row
		FString ContextString;
		TArray<FName> RowNames = ItemDataTable->GetRowNames();
		for (const FName& RowName : RowNames)
		{
			FSFItemData* ItemData = ItemDataTable->FindRow<FSFItemData>(RowName, ContextString);

			if (ItemData && ItemData->ItemClass)
			{
				USFItemBase* NewItem = NewObject<USFItemBase>(this, ItemData->ItemClass);
				if (NewItem)
				{
					NewItem->ItemName =ItemData->ItemName;
					NewItem->ItemDescription = FText::FromString(ItemData->ItemDescription);
					NewItem->Price = ItemData->Price;
					NewItem->ItemIcon = ItemData->ItemIcon.LoadSynchronous(); 
					ShopItems.Add(NewItem);
				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("Failed to create item object for Row: %s"), *RowName.ToString());
				}
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Invalid ItemClass in DataTable for Row: %s"), *RowName.ToString());
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to load Item Data Table: SFItemData"));
	}


	PopulateItemList();
}

void UShopItemListMenu::NativeDestruct()
{
    Super::NativeDestruct();
}

float UShopItemListMenu::GetCurrentGold()
{
    if (APlayerController* PlayerController = GetOwningPlayer())
    {
        if (ASFCharacter* PlayerCharacter = Cast<ASFCharacter>(PlayerController->GetPawn()))
        {
            if (ASFPlayerState* PlayerState = PlayerCharacter->GetPlayerState<ASFPlayerState>())
            {
                return PlayerState->CurrentGold;
            }
        }
    }
    return 0.0f;
}

void UShopItemListMenu::PurchaseSelectedItem()
{
	float CurrentGold = GetCurrentGold();

	if (SelectedItem && CurrentGold >= SelectedItem->Price)
	{
		if (APlayerController* PlayerController = GetOwningPlayer())
		{
			if (ASFCharacter* PlayerCharacter = Cast<ASFCharacter>(PlayerController->GetPawn()))
			{
				if (ASFPlayerState* PlayerState = PlayerCharacter->GetPlayerState<ASFPlayerState>())
				{
					if (PlayerState->RemoveGold(SelectedItem->Price))
					{
						UE_LOG(LogTemp, Warning, TEXT("Purchased %s for %d gold. Remaining gold: %f"), *SelectedItem->ItemName.ToString(), SelectedItem->Price, PlayerState->CurrentGold);

						UE_LOG(LogTemp, Warning, TEXT("Attempting to give item: %s"), *SelectedItem->GetName());

						UpdateCurrentGoldText();
						PurchaseButton->SetIsEnabled(false);
						SelectedItem = nullptr;
						ItemListView->ClearSelection();
					}
					else
					{
						UE_LOG(LogTemp, Warning, TEXT("Failed to remove gold for purchase."));
					}
				}
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Not enough gold to purchase %s. Current gold: %f, Required: %d"), SelectedItem ? *SelectedItem->ItemName.ToString() : TEXT("None"), CurrentGold, SelectedItem ? SelectedItem->Price : 0);
	}
}

void UShopItemListMenu::PopulateItemList()
{
	if (ItemListView)
	{
		ItemListView->ClearListItems();
		for (USFItemBase* Item : ShopItems)
		{
			ItemListView->AddItem(Item);
		}
	}
}

void UShopItemListMenu::OnItemSelectionChanged(UObject* SelectedItemObject)
{
    if (SelectedItemObject)
    {
        SelectedItem = Cast<USFItemBase>(SelectedItemObject);
        PurchaseButton->SetIsEnabled(true);
    }
    else
    {
        PurchaseButton->SetIsEnabled(false);
        SelectedItem = nullptr;
    }
}

void UShopItemListMenu::OnPurchaseButtonClicked()
{
    PurchaseSelectedItem();
}


void UShopItemListMenu::OnExitClicked()
{
    if (UUIManager* UIManager = ResolveUIManager())
    {
        UIManager->ShowLobbyMenu();
    }
}

void UShopItemListMenu::UpdateCurrentGoldText()
{
	if (CurrentGoldTextBlock)
	{
		CurrentGoldTextBlock->SetText(FText::AsNumber(GetCurrentGold()));
	}
}
