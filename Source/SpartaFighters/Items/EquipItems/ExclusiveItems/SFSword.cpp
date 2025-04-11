


#include "Items/EquipItems/ExclusiveItems/SFSword.h"

USFSword::USFSword()
{
	EquipSlot = SFEquipSlot::ExclusiveSlot; 
	AttackDamage = 15.0f;
	ItemName ="Sword";
	ItemDescription = FText::FromString(TEXT("Unlock sword skill"));
}