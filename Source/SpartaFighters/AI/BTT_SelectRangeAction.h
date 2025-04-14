#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_SelectRangeAction.generated.h"

/**
 * 
 */
UCLASS()
class SPARTAFIGHTERS_API UBTT_SelectRangeAction : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTT_SelectRangeAction();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector AIActionKey;

	UPROPERTY(EditAnywhere, Category = Blackboard)
	FBlackboardKeySelector TargetStateKey;
};
