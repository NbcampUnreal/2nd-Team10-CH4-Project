#pragma once

#include "CoreMinimal.h"
#include "Skill/SkillEffectBase.h"
#include "BuffEffect.generated.h"

UCLASS()
class SPARTAFIGHTERS_API UBuffEffect : public USkillEffectBase
{
	GENERATED_BODY()
	
public:
	virtual void ApplyEffect(AActor* Target, float FinalPower) override;
};
