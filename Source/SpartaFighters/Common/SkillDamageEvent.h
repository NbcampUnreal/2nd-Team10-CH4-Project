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

	// �ʼ�: ���� ���� �� �⺻ ���� ����
	virtual bool IsOfType(int32 InID) const override
	{
		return (FSkillDamageEvent::ClassID == InID) || FPointDamageEvent::IsOfType(InID);
	}

	static const int32 ClassID = 2024;
};