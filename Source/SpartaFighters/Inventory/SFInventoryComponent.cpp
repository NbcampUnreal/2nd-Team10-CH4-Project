


#include "Inventory/SFInventoryComponent.h"
#include "Character/SFCharacter.h"

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

//void USFInventoryComponent::UseConsumable()
//{
//	//subtract count by 1 if count is not 1
//	//UpdateData(); at the end
//	AActor* PlayerCharacter = GetOwner();
//	if (PlayerCharacter)
//	{
//		ASFCharacter* SFPlayerCharacter = Cast<ASFCharacter>(PlayerCharacter);
//		if (!SFPlayerCharacter) //|| ItemCount <= 0)
//		{
//			return;
//		}
//		//ItemCount--;
//		//SFPlayerCharacter->ApplyConsumableEffect();
//		UpdateData();
//	}
//}

//bool to check operation
bool USFInventoryComponent::AddItem(const FName& ItemName, const FSoftObjectPath& ItemIconPath, FText ItemDescription, EItemType ItemType, int32 Quantity)
{
	USFItemBase* ExistingItem = FindItemByName(ItemName);
	if (ExistingItem)
	{
		ExistingItem->ItemQuantity += Quantity;
		UpdateData();
		return true;
	}
	else
	{
		USFItemBase* NewItem = NewObject<USFItemBase>(this);
		if (NewItem)
		{
			NewItem->SetItemData(ItemName, ItemIconPath, ItemDescription, ItemType, Quantity);
			Inventory.Add(NewItem);
			UpdateData();
			return true;
		}
	}

	return false;
}

//bool to check operation
bool USFInventoryComponent::RemoveItem(FName ItemNameToRemove, int32 QuantityToRemove)
{
	for (int32 i = 0; i < Inventory.Num(); ++i)
	{
		if (Inventory[i]->ItemName == ItemNameToRemove)
		{
			if (Inventory[i]->ItemQuantity > QuantityToRemove)
			{
				Inventory[i]->ItemQuantity -= QuantityToRemove;
				UpdateData();
				return true;
			}
			else if (Inventory[i]->ItemQuantity == QuantityToRemove)
			{
				Inventory.RemoveAt(i);
				UpdateData();
				return true;
			}
			else // Inventory[i]->ItemQuantity < QuantityToRemove
			{
				QuantityToRemove -= Inventory[i]->ItemQuantity;
				Inventory.RemoveAt(i);
				i--; // arraysize-- index--
				UpdateData();
				if (QuantityToRemove == 0)
				{
					return true;
				}
			}
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

