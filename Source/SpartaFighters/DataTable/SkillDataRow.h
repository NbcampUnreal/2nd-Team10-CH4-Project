#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "SkillDataRow.generated.h"

class UAnimMontage;
class USkillEffectBase;
USTRUCT(BlueprintType)
struct FSkillDataRow : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName SkillName; 
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<UAnimMontage> SkillMontage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AttackPower;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MPCost;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CoolTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float KnockbackPower;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<USkillEffectBase> SkillEffectClass;
};
