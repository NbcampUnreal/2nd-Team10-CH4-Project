


#include "Items/EquipItems/CommonItems/SFNecklace.h"

USFNecklace::USFNecklace()
{
	EquipSlot = SFEquipSlot::Neck; //Equipable in slot Neck
	AdditionalAttackPower = 5.0f;
	ItemName ="Necklace";
	ItemDescription = FText::FromString(TEXT("Seems to be old"));
}