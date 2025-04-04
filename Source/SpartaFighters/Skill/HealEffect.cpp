#include "Skill/HealEffect.h"
#include "GameFramework/Actor.h"
#include "Character/SFCharacter.h"

void UHealEffect::ApplyEffect(AActor* Target, float FinalPower)
{
	if (Target)
	{
		ASFCharacter* Character = Cast<ASFCharacter>(Target);
		if (Character)
		{
			// TODO : Implement healing logic
			ensureAlways(false);
		}
	}
}