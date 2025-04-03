#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SkillEffectBase.generated.h"

UCLASS()
class SPARTAFIGHTERS_API USkillEffectBase : public UObject
{
	GENERATED_BODY()
	
public:
	virtual void ApplyEffect(AActor* TargetActor, float FinalPower) PURE_VIRTUAL(USkillEffectBase::ApplyEffect, ;);
};
