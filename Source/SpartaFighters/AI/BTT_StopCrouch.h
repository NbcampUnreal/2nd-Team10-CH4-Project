

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_StopCrouch.generated.h"

/**
 * 
 */
UCLASS()
class SPARTAFIGHTERS_API UBTT_StopCrouch : public UBTTaskNode
{
    GENERATED_BODY()

public:
    UBTT_StopCrouch();

protected:
    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
