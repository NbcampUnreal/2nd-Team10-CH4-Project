#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_BossMagicAttack.generated.h"

UCLASS()
class SPARTAFIGHTERS_API UBTT_BossMagicAttack : public UBTTaskNode
{
	GENERATED_BODY()

public:
    UBTT_BossMagicAttack();

    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
