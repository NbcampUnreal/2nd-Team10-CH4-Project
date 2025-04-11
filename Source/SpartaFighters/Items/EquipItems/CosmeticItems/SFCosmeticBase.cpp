


#include "Items/EquipItems/CosmeticItems/SFCosmeticBase.h"

USFCosmeticBase::USFCosmeticBase()
{
	ItemType = EItemType::Cosmetic;
	EquipSlot = SFEquipSlot::CosmeticSlot;
}

void USFCosmeticBase::ApplyCosmetic_Implementation(AActor* Owner)
{
	if (Owner)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s cosmetic attached. - owner: %s"), *ItemName.ToString(), *Owner->GetName());
	}
}

void USFCosmeticBase::RemoveCosmetic_Implementation(AActor* Owner)
{
	if (Owner)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s comsmetic detatched. - owner: %s"), *ItemName.ToString(), *Owner->GetName());
	}
}