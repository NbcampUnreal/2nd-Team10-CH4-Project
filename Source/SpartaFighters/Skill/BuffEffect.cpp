#include "Skill/BuffEffect.h"
#include "GameFramework/Actor.h"
#include "Character/SFCharacter.h"

void UBuffEffect::ApplyEffect(AActor* Target, float FinalPower)
{
	if (Target)
	{
		ASFCharacter* Character = Cast<ASFCharacter>(Target);
		if (Character)
		{
			// TODO : Implement buff logic 
			ensureAlways(false);
		}
	}
}
