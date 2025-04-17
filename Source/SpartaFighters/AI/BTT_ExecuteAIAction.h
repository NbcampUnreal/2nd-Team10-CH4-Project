#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_ExecuteAIAction.generated.h"

UCLASS()
class SPARTAFIGHTERS_API UBTT_ExecuteAIAction : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
    UBTT_ExecuteAIAction();

protected:
    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
    UPROPERTY(EditAnywhere, Category = "Blackboard")
    struct FBlackboardKeySelector AIActionKey;
};
