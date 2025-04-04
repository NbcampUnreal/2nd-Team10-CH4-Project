


#include "Items/ConsumableItems/SFHealthPotion.h"
#include "Character/SFCharacter.h"


void USFHealthPotion::ApplyConsumableEffect_Implementation(ASFCharacter* InPlayerCharacter)
{
	Super::ApplyConsumableEffect_Implementation(InPlayerCharacter);

	if (InPlayerCharacter)
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