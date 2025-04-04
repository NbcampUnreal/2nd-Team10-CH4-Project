
#include "Inventory/SFInventoryComponent.h"
#include "Character/SFCharacter.h"
#include "Items/EquipItems/SFEquipableBase.h"

// Sets default values for this component's properties
USFInventoryComponent::USFInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
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

void USFInventoryComponent::UpdateData()
{
	//Todo
	//get game instance
	UGameInstance* Instance = GetWorld()->GetGameInstance();
	if (Instance)
	{
		/*USFGameInstance* SFGameInstance = Cast<USFGameInstance>(Instance);
		if (SFGameInstance)
		{
			SFGameInstance->Has... = Potion;
		}*/
	}
}



//bool to check operation
bool USFInventoryComponent::AddItemByClass(TSubclassOf<USFItemBase> ItemClass)
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
				UpdateData();
				return true;
			}
		}
		else
		{
			return true; // already existing
		}
	}
	return false; //no class
}

//bool to check operation
bool USFInventoryComponent::RemoveItem(FName ItemNameToRemove)
{
	for (int32 i = 0; i < Inventory.Num(); ++i)
	{
		if (Inventory[i]->ItemName == ItemNameToRemove)
		{
			Inventory.RemoveAt(i);
			UpdateData();
			return true;
		}
	}
	return false;
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


bool USFInventoryComponent::EquipItem(FName ItemNameToEquip, SFEquipSlot EquipSlot)
{
	USFItemBase* ItemToEquip = FindItemByName(ItemNameToEquip);
	if (ItemToEquip && ItemToEquip->IsA(USFEquipableBase::StaticClass()))
	{
		USFEquipableBase* EquipItemToEquip = Cast<USFEquipableBase>(ItemToEquip);
		if (EquipItemToEquip->EquipSlot == EquipSlot)
		{
			if (EquippedItems.Contains(EquipSlot))
			{
				UnequipItem(EquipSlot);
			}

			if (RemoveItem(ItemNameToEquip))
			{
				EquippedItems.Add(EquipSlot, EquipItemToEquip);
				EquipItemToEquip->OnEquipped(GetOwner());
				UpdateData();
				return true;
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
	return false;
}

bool USFInventoryComponent::UnequipItem(SFEquipSlot EquipSlot)
{
	if (EquippedItems.Contains(EquipSlot))
	{
		USFItemBase* ItemToUnequip = EquippedItems[EquipSlot];
		if (ItemToUnequip && ItemToUnequip->IsA(USFEquipableBase::StaticClass()))
		{
			USFEquipableBase* EquipItemToUnequip = Cast<USFEquipableBase>(ItemToUnequip);
			EquipItemToUnequip->OnUnequipped(GetOwner());
			EquippedItems.Remove(EquipSlot);
			if (AddItemByClass(ItemToUnequip->GetClass()))
			{
				UpdateData();
				return true;
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Failed to unequip item into inventory: %s"), *ItemToUnequip->GetName());
				EquippedItems.Add(EquipSlot, ItemToUnequip);
				UpdateData();
				return false;
			}
		}
		EquippedItems.Remove(EquipSlot);
		UpdateData();
		return true;
	}
	return false;
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