

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_BossFireBreath.generated.h"

/**
 * 
 */
UCLASS()
class SPARTAFIGHTERS_API UBTT_BossFireBreath : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTT_BossFireBreath();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
