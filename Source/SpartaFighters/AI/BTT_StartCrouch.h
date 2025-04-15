

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_StartCrouch.generated.h"

/**
 * 
 */
UCLASS()
class SPARTAFIGHTERS_API UBTT_StartCrouch : public UBTTaskNode
{
    GENERATED_BODY()

public:
    UBTT_StartCrouch();

protected:
    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
