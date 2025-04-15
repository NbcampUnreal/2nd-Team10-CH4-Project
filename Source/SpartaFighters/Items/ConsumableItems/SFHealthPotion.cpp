#include "Items/ConsumableItems/SFHealthPotion.h"
#include "Character/SFCharacter.h"
#include "Character/Components/StatusComponent.h"


void USFHealthPotion::Server_ApplyConsumableEffect_Implementation(ASFCharacter* InPlayerCharacter)
{
	Super::Server_ApplyConsumableEffect_Implementation(InPlayerCharacter);

	if (InPlayerCharacter&&InPlayerCharacter->GetLocalRole() == ROLE_Authority)
	{
		if (UStatusComponent* StatusComp = InPlayerCharacter->FindComponentByClass<UStatusComponent>())
		{
			StatusComp->ModifyHP(RecoveryAmount);
			UE_LOG(LogTemp, Warning, TEXT("%s activated."), *ItemName.ToString(), RecoveryAmount);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Character '%s' does not have a UStatusComponent."), *InPlayerCharacter->GetName());
		}
		
	}
}

USFHealthPotion::USFHealthPotion()
{
	ItemName ="HealthPotion";
	ItemDescription = FText::FromString(TEXT("Recover health."));
	RecoveryAmount = 50.0f;
}