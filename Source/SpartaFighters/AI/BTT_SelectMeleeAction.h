

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_SelectMeleeAction.generated.h"

/**
 * 
 */
UCLASS()
class SPARTAFIGHTERS_API UBTT_SelectMeleeAction : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTT_SelectMeleeAction();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	UPROPERTY(EditAnywhere, Category = "Blackboard")
	struct FBlackboardKeySelector AIActionKey;

};
