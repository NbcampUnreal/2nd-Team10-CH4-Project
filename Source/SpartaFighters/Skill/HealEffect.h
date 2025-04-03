#pragma once

#include "CoreMinimal.h"
#include "Skill/SkillEffectBase.h"
#include "HealEffect.generated.h"

UCLASS()
class SPARTAFIGHTERS_API UHealEffect : public USkillEffectBase
{
	GENERATED_BODY()
	
public:
	virtual void ApplyEffect(AActor* Target, float FinalPower) override;
};
