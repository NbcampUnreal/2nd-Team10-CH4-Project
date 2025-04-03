

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "Navigation/PathFollowingComponent.h"
#include "BTT_MoveToTarget.generated.h"

/**
 * 
 */
UCLASS()
class SPARTAFIGHTERS_API UBTT_MoveToTarget : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
    UBTT_MoveToTarget();

    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

    UFUNCTION()
    void OnMoveCompleted(FAIRequestID RequestID, EPathFollowingResult::Type Result);

protected:

private:
    UPROPERTY(EditAnywhere, Category = "Blackboard")
    struct FBlackboardKeySelector TargetKey;

    UPROPERTY(EditAnywhere, Category = "Movement")
    float AcceptanceRadius;

    UBehaviorTreeComponent* BTComponent;

    bool bTaskFinished;

    FTimerHandle TimerHandle_MoveTimeout;
};
