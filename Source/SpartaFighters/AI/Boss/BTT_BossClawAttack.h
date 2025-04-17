#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_BossClawAttack.generated.h"

UCLASS()
class SPARTAFIGHTERS_API UBTT_BossClawAttack : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTT_BossClawAttack();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
