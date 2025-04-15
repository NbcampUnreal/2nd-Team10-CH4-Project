#include "Inventory/SFInventoryComponent.h"
#include "Character/SFCharacter.h"
#include "Items/EquipItems/SFEquipableBase.h"
#include "Net/UnrealNetwork.h"
#include "Framework/SFPlayerState.h"
#include "Framework/SFGameInstanceSubsystem.h"

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
					OwnerState->SetCharacterInventory(Inventory);
					OwnerState->SetCharacterEquipment(EquippedCommon, EquippedExclusive, EquippedCosmetic);

					// Save inventory info on GameInstance 
					UGameInstance* GameInstance = GetWorld()->GetGameInstance();
					if (GameInstance)
					{
						USFGameInstanceSubsystem* SFGameInstanceSubsystem = GameInstance->GetSubsystem<USFGameInstanceSubsystem>();
						if (SFGameInstanceSubsystem)
						{
							SFGameInstanceSubsystem->UpdatePlayerInventory(OwnerState->GetUniqueID(), Inventory);
							SFGameInstanceSubsystem->UpdatePlayerEquipment(OwnerState->GetUniqueID(), EquippedCommon, EquippedExclusive, EquippedCosmetic);
						}
					}
				}
			}
		}
	}
}



//server rpc: add item request
void USFInventoryComponent::Server_AddItemByClass_Implementation(TSubclassOf<USFItemBase> ItemClass)
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
					Internal_UpdateData();
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
void USFInventoryComponent::Server_RemoveItem_Implementation(FName ItemNameToRemove)
{
	if (GetOwnerRole() == ROLE_Authority)
	{
		for (int32 i = 0; i < Inventory.Num(); ++i)
		{
			if (Inventory[i]->ItemName == ItemNameToRemove)
			{
				Inventory.RemoveAt(i);
				Internal_UpdateData();
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


void USFInventoryComponent::Server_EquipItem_Implementation(FName ItemNameToEquip, SFEquipSlot EquipSlot)
{
	if (GetOwnerRole() == ROLE_Authority) 
	{
		USFItemBase* ItemToEquip = FindItemByName(ItemNameToEquip);
		if (ItemToEquip && ItemToEquip->IsA(USFEquipableBase::StaticClass()))
		{
			USFEquipableBase* EquipItemToEquip = Cast<USFEquipableBase>(ItemToEquip);
			if (EquipItemToEquip->EquipSlot == EquipSlot)
			{
				USFEquipableBase** EquippedSlot = GetEquippedItemPtrBySlot(EquipSlot);
				if (EquippedSlot)
				{
					//Unequip if slot already has item
					if (*EquippedSlot)
					{
						Server_UnequipItem_Implementation(EquipSlot);
					}

					if (Inventory.RemoveSingleSwap(ItemToEquip))
					{
						*EquippedSlot = EquipItemToEquip;
						EquipItemToEquip->OnEquipped(GetOwner());
						Internal_UpdateData();
						bInventoryUpdated = !bInventoryUpdated;
						bEquippedItemsUpdated = !bEquippedItemsUpdated;
					}
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

void USFInventoryComponent::Server_UnequipItem_Implementation(SFEquipSlot EquipSlot)
{
	if (GetOwnerRole() == ROLE_Authority) 
	{
		USFEquipableBase** EquippedSlot = GetEquippedItemPtrBySlot(EquipSlot);
		if (EquippedSlot && *EquippedSlot)
		{
			USFEquipableBase* ItemToUnequip = *EquippedSlot;
			*EquippedSlot = nullptr;

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
						Internal_UpdateData();
						bInventoryUpdated = !bInventoryUpdated;
					}
				}
				else
				{
					bInventoryUpdated = !bInventoryUpdated; //already existing
				}
			}
			ItemToUnequip->OnUnequipped(GetOwner());
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


const USFEquipableBase* USFInventoryComponent::GetEquippedItem(SFEquipSlot EquipSlot) const
{
	return GetEquippedItemBySlotInternal(EquipSlot);
}

//Find out whether item is equipped
bool USFInventoryComponent::IsItemEquipped(FName ItemName) const
{
	if (EquippedCommon && EquippedCommon->ItemName == ItemName) return true;
	if (EquippedExclusive && EquippedExclusive->ItemName == ItemName) return true;
	if (EquippedCosmetic && EquippedCosmetic->ItemName == ItemName) return true;
	return false;
}

//Lifetime
void USFInventoryComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(USFInventoryComponent, Inventory);
	DOREPLIFETIME(USFInventoryComponent, bInventoryUpdated);
	DOREPLIFETIME(USFInventoryComponent, bEquippedItemsUpdated);
	DOREPLIFETIME(USFInventoryComponent, EquippedCommon);
	DOREPLIFETIME(USFInventoryComponent, EquippedExclusive);
	DOREPLIFETIME(USFInventoryComponent, EquippedCosmetic);

}

//Inv UI Update
void USFInventoryComponent::OnRep_InventoryUpdated()
{
	//OnInventoryUpdated.Broadcast();
}

//Eq UI Update
void USFInventoryComponent::OnRep_EquippedItemsUpdated()
{
	//OnEquippedItemsUpdated.Broadcast();
}

//Return equipped item pointer
USFEquipableBase** USFInventoryComponent::GetEquippedItemPtrBySlot(SFEquipSlot EquipSlot)
{
	switch (EquipSlot)
	{
	case SFEquipSlot::CommonSlot: return &EquippedCommon;
	case SFEquipSlot::ExclusiveSlot: return &EquippedExclusive;
	case SFEquipSlot::CosmeticSlot: return &EquippedCosmetic;
	default: return nullptr;
	}
}

//Return equipped item
const USFEquipableBase* USFInventoryComponent::GetEquippedItemBySlotInternal(SFEquipSlot EquipSlot) const
{
	switch (EquipSlot)
	{
	case SFEquipSlot::CommonSlot: return EquippedCommon;
	case SFEquipSlot::ExclusiveSlot: return EquippedExclusive;
	case SFEquipSlot::CosmeticSlot: return EquippedCosmetic;
	default: return nullptr;
	}
}