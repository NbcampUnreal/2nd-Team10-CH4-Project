

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_SelectRandomTarget.generated.h"

/**
 * 
 */
UCLASS()
class SPARTAFIGHTERS_API UBTT_SelectRandomTarget : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
    UBTT_SelectRandomTarget();

protected:
    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
    UPROPERTY(EditAnywhere, Category = "Blackboard")
    struct FBlackboardKeySelector TargetKey;

    // if use othermode
    //UPROPERTY(EditAnywhere, Category = "Target Selection")
    //bool bSelectOnlyPlayers;

    //UPROPERTY(EditAnywhere, Category = "Target Selection")
    //bool bSelectOnlyAI;

};
