

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_StopJump.generated.h"

/**
 * 
 */
UCLASS()
class SPARTAFIGHTERS_API UBTT_StopJump : public UBTTaskNode
{
    GENERATED_BODY()

public:
    UBTT_StopJump();

protected:
    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
