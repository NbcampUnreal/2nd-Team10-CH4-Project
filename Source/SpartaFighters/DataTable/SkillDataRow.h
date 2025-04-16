#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "SkillDataRow.generated.h"

//UENUM(BlueprintType)
//enum class ECollisionType : uint8
//{
//	Sphere          UMETA(DisplayName = "Sphere Collision"),
//	Box				UMETA(DisplayName = "Box Collision"),
//	Capsule         UMETA(DisplayName = "Box Collision"),
//};


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
	TObjectPtr<UAnimMontage> SkillMontage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AttackPower;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MPCost;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CoolTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float KnockbackPower;

	// Heat Tracing
	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//ECollisionType CollisionType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName SocketLocation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float TraceLength;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float TraceRadius;


	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//TSubclassOf<USkillEffectBase> SkillEffectClass;
};
