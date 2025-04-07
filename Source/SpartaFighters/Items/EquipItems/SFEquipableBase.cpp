


#include "Items/EquipItems/SFEquipableBase.h"

USFEquipableBase::USFEquipableBase()
{
	ItemType = EItemType::Common;
	EquipSlot = SFEquipSlot::Head;
}

void USFEquipableBase::OnEquipped_Implementation(AActor* Owner)
{
	if (Owner)
	{
		UE_LOG(LogTemp, Warning, TEXT("Equipped %s in slot %s . - Owner: %s"), *ItemName.ToString(), *UEnum::GetValueAsString(EquipSlot), *Owner->GetName());
	}
}

void USFEquipableBase::OnUnequipped_Implementation(AActor* Owner)
{
	if (Owner)
	{
		UE_LOG(LogTemp, Warning, TEXT("Unequipped %s from slot %s . - Owner: %s"), *ItemName.ToString(), *UEnum::GetValueAsString(EquipSlot), *Owner->GetName());
	}
}