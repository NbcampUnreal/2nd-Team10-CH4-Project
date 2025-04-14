#include "Items/ConsumableItems/SFConsumableBase.h"
#include "Net/UnrealNetwork.h"

USFConsumableBase::USFConsumableBase()
{
	ItemType = EItemType::Consumable;
}

void USFConsumableBase::Server_ApplyConsumableEffect_Implementation(ASFCharacter* InPlayerCharacter)
{
	if (InPlayerCharacter && InPlayerCharacter->GetLocalRole() == ROLE_Authority)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s applied in server"), *ItemName.ToString());
		// ex) InPlayerCharacter->Heal(HealAmount);
	}
}