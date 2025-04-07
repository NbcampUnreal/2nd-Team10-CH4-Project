


#include "Items/ConsumableItems/SFConsumableBase.h"

USFConsumableBase::USFConsumableBase()
{
	ItemType = EItemType::Consumable;
}

void USFConsumableBase::ApplyConsumableEffect_Implementation(ASFCharacter* InPlayerCharacter)
{
	UE_LOG(LogTemp, Warning, TEXT("%s applied"), *ItemName.ToString());
}