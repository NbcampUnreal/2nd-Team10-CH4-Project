#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_CheckDistance.generated.h"


UCLASS()
class SPARTAFIGHTERS_API UBTT_CheckDistance : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTT_CheckDistance();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

public:
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	struct FBlackboardKeySelector TargetKey;

	UPROPERTY(EditAnywhere, Category = "Blackboard")
	struct FBlackboardKeySelector DistanceTypeKey;

	UPROPERTY(EditAnywhere, Category = "Distance")
	float MeleeRange;

	UPROPERTY(EditAnywhere, Category = "Distance")
	float RangedRange;
};
