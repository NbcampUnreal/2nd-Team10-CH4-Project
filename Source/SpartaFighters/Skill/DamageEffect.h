#pragma once

#include "CoreMinimal.h"
#include "Skill/SkillEffectBase.h"
#include "DamageEffect.generated.h"

UCLASS()
class SPARTAFIGHTERS_API UDamageEffect : public USkillEffectBase
{
	GENERATED_BODY()
	
public:
	virtual void ApplyEffect(AActor* Target, float FinalPower) override;
};
