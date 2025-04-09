
#include "Inventory/SFInventoryComponent.h"
#include "Character/SFCharacter.h"
#include "Items/EquipItems/SFEquipableBase.h"
#include "Net/UnrealNetwork.h"
#include "Framework/SFPlayerState.h"


USFInventoryComponent::USFInventoryComponent()
{
	SetIsReplicatedByDefault(true);
	PrimaryComponentTick.bCanEverTick = false;

}


// Called when the game starts
void USFInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void USFInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void USFInventoryComponent::Internal_UpdateData()
{
	if (GetOwnerRole() == ROLE_Authority)
	{
		ASFCharacter* OwnerCharacter = Cast<ASFCharacter>(GetOwner());
		if (OwnerCharacter)
		{
			// Get Player Controller
			APlayerController* OwnerController = OwnerCharacter->GetController<APlayerController>();
			if (OwnerController)
			{
				// Get Player State
				ASFPlayerState* OwnerState = OwnerController->GetPlayerState<ASFPlayerState>();
				if (OwnerState)
				{
					// Save inventory info on PlayerState
					// OwnerState->SetCharacterInventory(Inventory);
					// OwnerState->SetCharacterEquipment(EquippedItems);

					// Save inventory info on GameInstance 
					UGameInstance* GameInstance = GetWorld()->GetGameInstance();
					if (GameInstance)
					{
						// USFGameInstance* SFGameInstance = Cast<USFGameInstance>(GameInstance);
						// if (SFGameInstance)
						// {
						//     SFGameInstance->UpdatePlayerInventory(OwnerState->GetUniqueId(), Inventory);
						//     SFGameInstance->UpdatePlayerEquipment(OwnerState->GetUniqueId(), EquippedItems);
						// }
					}
				}
			}
		}
	}
}



//server rpc: add item request
void USFInventoryComponent::Server_AddItemByClass(TSubclassOf<USFItemBase> ItemClass)
{
	if (GetOwnerRole() == ROLE_Authority)
	{
		if (ItemClass)
		{
			FName DefaultItemName = ItemClass.GetDefaultObject()->ItemName;
			USFItemBase* ExistingItem = FindItemByName(DefaultItemName);
			if (!ExistingItem)
			{
				USFItemBase* NewItem = NewObject<USFItemBase>(this, ItemClass);
				if (NewItem)
				{
					Inventory.Add(NewItem);
					//Internal_UpdateData();
					bInventoryUpdated = !bInventoryUpdated;
				}
			}
			else
			{
				bInventoryUpdated = !bInventoryUpdated; // already existing
			}
		} //no class
	}
}

//server rpc: remove item request
void USFInventoryComponent::Server_RemoveItem(FName ItemNameToRemove)
{
	if (GetOwnerRole() == ROLE_Authority)
	{
		for (int32 i = 0; i < Inventory.Num(); ++i)
		{
			if (Inventory[i]->ItemName == ItemNameToRemove)
			{
				Inventory.RemoveAt(i);
				//Internal_UpdateData();
				bInventoryUpdated = !bInventoryUpdated;
				return;
			}
		}
		//no item
	}
}
	

USFItemBase* USFInventoryComponent::FindItemByName(FName Name) const
{
	for (USFItemBase* Item : Inventory)
	{
		if (Item && Item->ItemName == Name)
		{
			return Item;
		}
	}
	return nullptr;
}


void USFInventoryComponent::Server_EquipItem(FName ItemNameToEquip, SFEquipSlot EquipSlot)
{
	if (GetOwnerRole() == ROLE_Authority) 
	{
		USFItemBase* ItemToEquip = FindItemByName(ItemNameToEquip);
		if (ItemToEquip && ItemToEquip->IsA(USFEquipableBase::StaticClass()))
		{
			USFEquipableBase* EquipItemToEquip = Cast<USFEquipableBase>(ItemToEquip);
			if (EquipItemToEquip->EquipSlot == EquipSlot)
			{
				if (EquippedItems.Contains(EquipSlot))
				{
					Server_UnequipItem(EquipSlot);
				}

				if (Inventory.RemoveSingleSwap(ItemToEquip))
				{
					EquippedItems.Add(EquipSlot, EquipItemToEquip);
					EquipItemToEquip->OnEquipped(GetOwner());
					Internal_UpdateData();
					bInventoryUpdated = !bInventoryUpdated;
					bEquippedItemsUpdated = !bEquippedItemsUpdated;
				}
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("%s is unable to equip in %s slot"), *ItemNameToEquip.ToString(), *UEnum::GetValueAsString(EquipSlot));
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("%s is not equipable or not in inventory"), *ItemNameToEquip.ToString());
		}
	}
}

void USFInventoryComponent::Server_UnequipItem(SFEquipSlot EquipSlot)
{
	if (GetOwnerRole() == ROLE_Authority) 
	{
		if (EquippedItems.Contains(EquipSlot))
		{
			USFItemBase* ItemToUnequip = EquippedItems[EquipSlot];
			EquippedItems.Remove(EquipSlot);
			if (ItemToUnequip)
			{
				// Server_AddItemByClass 
				TSubclassOf<USFItemBase> ItemClass = ItemToUnequip->GetClass();
				if (ItemClass)
				{
					FName DefaultItemName = ItemClass.GetDefaultObject()->ItemName;
					USFItemBase* ExistingItem = FindItemByName(DefaultItemName);
					if (!ExistingItem)
					{
						USFItemBase* NewItem = NewObject<USFItemBase>(this, ItemClass);
						if (NewItem)
						{
							Inventory.Add(NewItem);
							//Internal_UpdateData();
							bInventoryUpdated = !bInventoryUpdated;
						}
					}
					else
					{
						bInventoryUpdated = !bInventoryUpdated; //already existing
					}
				}

				if (USFEquipableBase* EquipItemToUnequip = Cast<USFEquipableBase>(ItemToUnequip))
				{
					EquipItemToUnequip->OnUnequipped(GetOwner());
				}
				Internal_UpdateData();
				bEquippedItemsUpdated = !bEquippedItemsUpdated;
			}
			else
			{
				Internal_UpdateData();
				bEquippedItemsUpdated = !bEquippedItemsUpdated; //empty slot
			}
		}
	}
}

USFItemBase* USFInventoryComponent::GetEquippedItem(SFEquipSlot EquipSlot) const
{
	if (EquippedItems.Contains(EquipSlot))
	{
		return EquippedItems[EquipSlot];
	}
	return nullptr;
}

//To do 
// get rid of auto
bool USFInventoryComponent::IsItemEquipped(FName ItemName) const
{
	for (const auto& Pair : EquippedItems)
	{
		if (Pair.Value && Pair.Value->ItemName == ItemName)
		{
			return true;
		}
	}
	return false;
}

//Lifetime
void USFInventoryComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(USFInventoryComponent, Inventory);
	DOREPLIFETIME(USFInventoryComponent, bInventoryUpdated);
	DOREPLIFETIME(USFInventoryComponent, bEquippedItemsUpdated);

}

//Inv UI Update
void USFInventoryComponent::OnRep_InventoryUpdated()
{
	
}
//Eq UI Update
void USFInventoryComponent::OnRep_EquippedItemsUpdated()
{
	
}