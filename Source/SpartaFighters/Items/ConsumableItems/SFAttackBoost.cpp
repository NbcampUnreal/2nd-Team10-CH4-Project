#include "Items/ConsumableItems/SFAttackBoost.h"
#include "Character/SFCharacter.h"
#include "TimerManager.h"
#include "Character/Components/StatusComponent.h"



USFAttackBoost::USFAttackBoost()
{
	ItemName = "AttackBoost";
	ItemDescription = FText::FromString(TEXT("Attack Boost"));
	AttackBoostAmount = 10.0f;
	EffectDuration = 10.0f;
}

void USFAttackBoost::Server_ApplyConsumableEffect_Implementation(ASFCharacter* InPlayerCharacter)
{
	Super::Server_ApplyConsumableEffect_Implementation(InPlayerCharacter);

	if (InPlayerCharacter&&InPlayerCharacter->GetLocalRole() == ROLE_Authority)
	{
		if (UStatusComponent* StatusComp = InPlayerCharacter->FindComponentByClass<UStatusComponent>())
		{
			StatusComp->ModifyAttackPower(AttackBoostAmount);
			UE_LOG(LogTemp, Warning, TEXT("%s used! Attackpower increased by %f . (Duration %f)"), *ItemName.ToString(), AttackBoostAmount, EffectDuration);

			//return by timer
			FTimerHandle TimerHandle;
			FTimerDelegate TimerDelegate;
			TimerDelegate.BindUFunction(this, "RevertAttackBoost", InPlayerCharacter, AttackBoostAmount);
			InPlayerCharacter->GetWorldTimerManager().SetTimer(TimerHandle, TimerDelegate, EffectDuration, false);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Character '%s' does not have a UStatusComponent."), *InPlayerCharacter->GetName());
		}

	}
}

void USFAttackBoost::RevertAttackBoost(ASFCharacter* InPlayerCharacter, float Amount)
{
	if (InPlayerCharacter&& InPlayerCharacter->GetLocalRole() == ROLE_Authority)
	{
		if (UStatusComponent* StatusComp = InPlayerCharacter->FindComponentByClass<UStatusComponent>())
		{
			StatusComp->ModifyAttackPower(-Amount); //Negative amount
			UE_LOG(LogTemp, Warning, TEXT("%s Effect End. Attack power decreased by %f."), *ItemName.ToString(), Amount);
		}
	}
}