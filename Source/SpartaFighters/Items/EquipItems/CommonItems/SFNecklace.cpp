


#include "Items/EquipItems/CommonItems/SFNecklace.h"

USFNecklace::USFNecklace()
{
	AdditionalAttackPower = 5.0f;
	ItemName ="Necklace";
	ItemDescription = FText::FromString(TEXT("Seems to be old"));
	ItemIcon = TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/PlatformFighterKit/Assets/FX/Textures/T_wind")));
}