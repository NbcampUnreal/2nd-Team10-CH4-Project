#pragma once

#include "CoreMinimal.h"
#include "Skill/SkillEffectBase.h"
#include "TeleportEffect.generated.h"

UCLASS()
class SPARTAFIGHTERS_API UTeleportEffect : public USkillEffectBase
{
	GENERATED_BODY()
	
public:
	virtual void ApplyEffect(AActor* Target, float FinalPower) override;
};
