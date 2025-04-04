


#include "Items/EquipItems/ExclusiveItems/SFSword.h"

USFSword::USFSword()
{
	EquipSlot = SFEquipSlot::Hand; // 주 무기 슬롯
	AttackDamage = 15.0f;
	ItemName ="Sword";
	ItemDescription = FText::FromString(TEXT("Unlock sword skill"));
}