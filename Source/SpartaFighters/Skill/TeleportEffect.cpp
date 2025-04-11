#include "Skill/TeleportEffect.h"
#include "Character/Mage/SFMageCharacter.h"

void UTeleportEffect::ApplyEffect(AActor* Target, float FinalPower)
{
	if (Target)
	{
		ASFMageCharacter* MageCharacter = Cast<ASFMageCharacter>(Target);
	}
}
