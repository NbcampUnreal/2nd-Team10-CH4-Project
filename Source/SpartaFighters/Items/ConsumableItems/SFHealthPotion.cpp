#include "Items/ConsumableItems/SFHealthPotion.h"
#include "Character/SFCharacter.h"


void USFHealthPotion::Server_ApplyConsumableEffect_Implementation(ASFCharacter* InPlayerCharacter)
{
	Super::Server_ApplyConsumableEffect_Implementation(InPlayerCharacter);

	if (InPlayerCharacter&&InPlayerCharacter->GetLocalRole() == ROLE_Authority)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s activated."), *ItemName.ToString(), RecoveryAmount);
	}
}

USFHealthPotion::USFHealthPotion()
{
	ItemName ="HealthPotion";
	ItemDescription = FText::FromString(TEXT("Recover health."));
	RecoveryAmount = 50.0f;
}