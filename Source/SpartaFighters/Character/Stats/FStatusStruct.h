#pragma once

#include "CoreMinimal.h"
#include "EStatusType.h"
#include "FStatusStruct.generated.h"

USTRUCT(BlueprintType)
struct FStatusStruct
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<EStatusType, float> StatusValues;

    FStatusStruct()
    {
        StatusValues.Add(EStatusType::MaxHP, 100.f);
        StatusValues.Add(EStatusType::CurHP, Get(EStatusType::MaxHP));
        StatusValues.Add(EStatusType::MaxArmor, 25.f);
        StatusValues.Add(EStatusType::CurArmor, Get(EStatusType::MaxArmor));
        StatusValues.Add(EStatusType::MaxMP, 100.f);
        StatusValues.Add(EStatusType::CurMP, Get(EStatusType::MaxMP));
        StatusValues.Add(EStatusType::MaxKnockbackAmount, 50.f);
        StatusValues.Add(EStatusType::CurKnockbackAmount, 0.f);
        StatusValues.Add(EStatusType::KnockbackPower, 20.f);
        StatusValues.Add(EStatusType::AttackPower, 10.f);
        StatusValues.Add(EStatusType::DefensePower, 4.f);
        StatusValues.Add(EStatusType::MoveSpeed, 500.f);
        StatusValues.Add(EStatusType::JumpPower, 800.f);
    }

    float Get(EStatusType Type) const
    {
        if (const float* Value = StatusValues.Find(Type))
        {
            return *Value;
        }
        return 0.f;
    }

    void Set(EStatusType Type, float Value)
    {
        if (StatusValues.Contains(Type))
        {
            StatusValues[Type] = Value;
        }
    }
};