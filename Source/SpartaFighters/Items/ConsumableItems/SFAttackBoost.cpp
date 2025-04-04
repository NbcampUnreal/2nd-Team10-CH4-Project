
#include "Items/ConsumableItems/SFAttackBoost.h"
#include "Character/SFCharacter.h"
#include "TimerManager.h"


USFAttackBoost::USFAttackBoost()
{
	ItemName = "AttackBoost";
	ItemDescription = FText::FromString(TEXT("Attack Boost"));
	AttackBoostAmount = 10.0f;
	EffectDuration = 10.0f;
}

void USFAttackBoost::ApplyConsumableEffect_Implementation(ASFCharacter* InPlayerCharacter)
{
	Super::ApplyConsumableEffect_Implementation(InPlayerCharacter);

	if (InPlayerCharacter)
	{
		//InPlayerCharacter->IncreaseAttackPower(AttackBoostAmount);
		UE_LOG(LogTemp, Warning, TEXT("%s used! Attackpower increased by %f . (Duration %f)"), *ItemName.ToString(), AttackBoostAmount, EffectDuration);

		//return by timer
		FTimerHandle TimerHandle;
		FTimerDelegate TimerDelegate;
		TimerDelegate.BindUFunction(this, "RevertAttackBoost", InPlayerCharacter, AttackBoostAmount);
		InPlayerCharacter->GetWorldTimerManager().SetTimer(TimerHandle, TimerDelegate, EffectDuration, false);


	}
}

void USFAttackBoost::RevertAttackBoost(ASFCharacter* InPlayerCharacter, float Amount)
{
	if (InPlayerCharacter)
	{
		//InPlayerCharacter->DecreaseAttackPower(Amount);
		UE_LOG(LogTemp, Warning, TEXT("%s Effect End."), *ItemName.ToString());
	}
}