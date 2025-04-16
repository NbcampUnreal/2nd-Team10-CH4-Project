#pragma once

#include "CoreMinimal.h"
#include "Engine/DamageEvents.h"
#include "SkillDamageEvent.generated.h"

USTRUCT(BlueprintType)
struct FSkillDamageEvent : public FPointDamageEvent
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float KnockBackPower = 800.f;

	FSkillDamageEvent() : FPointDamageEvent()
		, KnockBackPower(800.f)
	{
	}

	virtual int32 GetTypeID() const override
	{
		return FSkillDamageEvent::ClassID;
	}

	// 필수: 복사 생성 시 기본 구조 복사
	virtual bool IsOfType(int32 InID) const override
	{
		return (FSkillDamageEvent::ClassID == InID) || FPointDamageEvent::IsOfType(InID);
	}

	static const int32 ClassID = 2024;
};