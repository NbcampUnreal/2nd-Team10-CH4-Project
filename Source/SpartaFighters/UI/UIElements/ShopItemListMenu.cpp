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
#include "Inventory/SFInventoryComponent.h" 
#include "Framework/SFGameInstanceSubsystem.h"

class UShopItemWidget;

void UShopItemListMenu::NativeConstruct()
{
    Super::NativeConstruct();

    if (PurchaseButton)
    {
        PurchaseButton->OnClicked.AddDynamic(this, &UShopItemListMenu::OnPurchaseButtonClicked);
        PurchaseButton->SetIsEnabled(false);
    }
	if (EquipButton)
	{
		EquipButton->OnClicked.AddDynamic(this, &UShopItemListMenu::OnEquipButtonClicked);
		EquipButton->SetIsEnabled(false);
	}
    if (ItemListView)
    {
        ItemListView->OnItemSelectionChanged().AddUObject(this, &UShopItemListMenu::OnItemSelectionChanged);
    }
	
	if (ItemDataTable && ShopItemTypeTextBlock)
	{
		//Go through Item Row
		FString ContextString;
		TArray<FName> RowNames = ItemDataTable->GetRowNames();
		FText ItemTypeText = ShopItemTypeTextBlock->GetText();
		EItemType FilterType = EItemType::Common;
		if (ItemTypeText.EqualTo(FText::FromString("CommonItem")))
		{
			FilterType = EItemType::Common;
		}
		else if (ItemTypeText.EqualTo(FText::FromString("ExclusiveItem")))
		{
			FilterType = EItemType::Exclusive;
		}
		else if (ItemTypeText.EqualTo(FText::FromString("CosmeticItem")))
		{
			FilterType = EItemType::Cosmetic;
		}

		for (const FName& RowName : RowNames)
		{
			FSFItemData* ItemData = ItemDataTable->FindRow<FSFItemData>(RowName, ContextString);
			
			if (ItemData && ItemData->ItemClass && ItemData->ItemType == FilterType)
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

	UpdateCurrentGoldText();
	PopulateItemList();
	UpdateEquipButtonVisibility();
}

void UShopItemListMenu::NativeDestruct()
{
    Super::NativeDestruct();
}

float UShopItemListMenu::GetCurrentGold()
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

void UShopItemListMenu::PurchaseSelectedItem()
{
	float CurrentGold = GetCurrentGold();

	if (SelectedItem && CurrentGold >= SelectedItem->Price)
	{
		if (APlayerController* PlayerController = GetOwningPlayer())
		{
			if (ASFPlayerState* PlayerState = PlayerController->GetPlayerState<ASFPlayerState>())
			{
				UE_LOG(LogTemp, Warning, TEXT("PlayerState UniqueID for purchase: %s"), *PlayerState->GetUniqueID());
				if (PlayerState->RemoveGold(SelectedItem->Price))
				{
					if (ASFCharacter* PlayerCharacter = Cast<ASFCharacter>(PlayerController->GetPawn()))
					{
						//When Character exist
						if (USFInventoryComponent* InventoryComponent = GetInventoryComponent())
						{
							InventoryComponent->Server_AddItemByClass(SelectedItem->GetClass());
							if (USFGameInstanceSubsystem* GameInstanceSubsystem = GetGameInstanceSubsystem())
							{
								GameInstanceSubsystem->UpdatePlayerInventory(PlayerState->GetUniqueID(), InventoryComponent->GetInventory());
							}
						}
						UE_LOG(LogTemp, Warning, TEXT("Purchased %s for %d gold (Character Exists)."), *SelectedItem->ItemName.ToString(), SelectedItem->Price);
					}
					else
					{
						//When Character doesnt exist
						if (USFGameInstanceSubsystem* GameInstanceSubsystem = GetGameInstanceSubsystem())
						{
							GameInstanceSubsystem->AddPendingShopPurchase(PlayerState->GetUniqueID(), SelectedItem->GetClass());
							UE_LOG(LogTemp, Warning, TEXT("Added %s to pending shop purchases (No Character)."), *SelectedItem->GetName());
						}
					}

					UpdateCurrentGoldText();
					PurchaseButton->SetIsEnabled(false);
					EquipButton->SetIsEnabled(false);
					SelectedItem = nullptr;
					ItemListView->ClearSelection();
				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("Failed to remove gold for purchase."));
				}
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("PurchaseSelectedItem - PlayerController is nullptr."));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Not enough gold to purchase %s."), SelectedItem ? *SelectedItem->ItemName.ToString() : TEXT("None"));
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
		UpdateEquipButtonVisibility();
    }
    else
    {
        PurchaseButton->SetIsEnabled(false);
		EquipButton->SetIsEnabled(false);
        SelectedItem = nullptr;
    }
}

void UShopItemListMenu::OnPurchaseButtonClicked()
{
	UE_LOG(LogTemp, Warning, TEXT("OnPurchaseButtonClicked called"));
    PurchaseSelectedItem();
}

//equip
void UShopItemListMenu::OnEquipButtonClicked()
{
	if (SelectedItem && SelectedItem->IsA(USFEquipableBase::StaticClass()))
	{
		USFEquipableBase* EquipableItem = Cast<USFEquipableBase>(SelectedItem);
		USFInventoryComponent* InventoryComponent = GetInventoryComponent();
		if (InventoryComponent)
		{
			// 현재 인벤토리에 해당 아이템이 있는지 확인
			USFItemBase* InventoryItem = InventoryComponent->FindItemByName(EquipableItem->ItemName);
			if (InventoryItem)
			{
				InventoryComponent->Server_EquipItem(EquipableItem->ItemName, EquipableItem->EquipSlot);
				if (USFGameInstanceSubsystem* GameInstanceSubsystem = GetGameInstanceSubsystem())
				{
					if (APlayerController* PlayerController = GetOwningPlayer())
					{
						if (ASFPlayerState* OwningPlayerState = PlayerController->GetPlayerState<ASFPlayerState>())
						{
							const USFEquipableBase* EquippedItem = InventoryComponent->GetEquippedItem(EquipableItem->EquipSlot);
							if (EquippedItem && EquippedItem->ItemName == EquipableItem->ItemName)
							{
								USFEquipableBase* Common = nullptr;
								USFEquipableBase* Exclusive = nullptr;
								USFEquipableBase* Cosmetic = nullptr;

								switch (EquipableItem->EquipSlot)
								{
								case SFEquipSlot::CommonSlot:
									Common = const_cast<USFEquipableBase*>(EquippedItem);
									break;
								case SFEquipSlot::ExclusiveSlot:
									Exclusive = const_cast<USFEquipableBase*>(EquippedItem);
									break;
								case SFEquipSlot::CosmeticSlot:
									Cosmetic = const_cast<USFEquipableBase*>(EquippedItem);
									break;
								default:
									break;
								}
								GameInstanceSubsystem->UpdatePlayerEquipment(OwningPlayerState->GetUniqueID(), Common, Exclusive, Cosmetic);
								UE_LOG(LogTemp, Log, TEXT("Updated equipment info in GameInstance for %s, Slot: %s"), *EquipableItem->ItemName.ToString(), *UEnum::GetValueAsString(EquipableItem->EquipSlot));
							}
						}
					}
				}
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("%s is not in the inventory."), *EquipableItem->ItemName.ToString());
			}
		}
	}
}


void UShopItemListMenu::OnExitClicked()
{
    if (UUIManager* UIManager = ResolveUIManager())
    {
        UIManager->ShowRoomMenu();
    }
}

void UShopItemListMenu::SetItemTypeTextBlock(EItemType Type)
{
	switch (Type)
	{
	case EItemType::Common:
		ShopItemTypeTextBlock->SetText(FText::FromString("CommonItem"));
		break;
	case EItemType::Exclusive:
		ShopItemTypeTextBlock->SetText(FText::FromString("ExclusiveItem"));
		break;
	case EItemType::Cosmetic:
		ShopItemTypeTextBlock->SetText(FText::FromString("CosmeticItem"));
		break;
	default:
		break;
	}
}

void UShopItemListMenu::UpdateCurrentGoldText()
{
	if (CurrentGoldTextBlock)
	{
		CurrentGoldTextBlock->SetText(FText::AsNumber(GetCurrentGold()));
	}
}

USFInventoryComponent* UShopItemListMenu::GetInventoryComponent()
{
	if (APlayerController* PlayerController = GetOwningPlayer())
	{
		if (ASFCharacter* PlayerCharacter = Cast<ASFCharacter>(PlayerController->GetPawn()))
		{
			return PlayerCharacter->FindComponentByClass<USFInventoryComponent>();
		}
	}
	return nullptr;
}

USFGameInstanceSubsystem* UShopItemListMenu::GetGameInstanceSubsystem()
{
	UGameInstance* GameInstance = GetGameInstance();
	if (GameInstance)
	{
		return GameInstance->GetSubsystem<USFGameInstanceSubsystem>();
	}
	return nullptr;
}

void UShopItemListMenu::UpdateEquipButtonVisibility()
{
	if (EquipButton && SelectedItem && SelectedItem->IsA(USFEquipableBase::StaticClass()))
	{
		EquipButton->SetVisibility(ESlateVisibility::Visible);
		USFInventoryComponent* InventoryComponent = GetInventoryComponent();
		if (InventoryComponent && InventoryComponent->FindItemByName(SelectedItem->ItemName))
		{
			EquipButton->SetIsEnabled(!InventoryComponent->IsItemEquipped(SelectedItem->ItemName));
			PurchaseButton->SetVisibility(ESlateVisibility::Visible);
		}
		else
		{
			EquipButton->SetIsEnabled(false);
			PurchaseButton->SetVisibility(ESlateVisibility::Visible);
		}
	}
	else if (EquipButton && SelectedItem)
	{
		// 장착 불가능한 아이템 선택 시 버튼 활성화 (클릭 시 피드백 제공)
		EquipButton->SetVisibility(ESlateVisibility::Visible);
		EquipButton->SetIsEnabled(true);
		PurchaseButton->SetVisibility(ESlateVisibility::Visible);
	}
	else if (EquipButton)
	{
		EquipButton->SetVisibility(ESlateVisibility::Visible);
		EquipButton->SetIsEnabled(false);
		PurchaseButton->SetVisibility(ESlateVisibility::Visible);
	}
}
