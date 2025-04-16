

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_RandomTypeAttack.generated.h"

// After the state is introduced, it needs to be completely overhauled.

UCLASS()
class SPARTAFIGHTERS_API UBTT_RandomTypeAttack : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTT_RandomTypeAttack();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

public:
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	struct FBlackboardKeySelector AttackTypeKey;
};
