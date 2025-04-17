#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_StartJump.generated.h"

UCLASS()
class SPARTAFIGHTERS_API UBTT_StartJump : public UBTTaskNode
{
    GENERATED_BODY()

public:
    UBTT_StartJump();

protected:
    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
