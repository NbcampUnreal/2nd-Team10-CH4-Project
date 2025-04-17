#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTS_CheckTargetState.generated.h"

UCLASS()
class SPARTAFIGHTERS_API UBTS_CheckTargetState : public UBTService
{
	GENERATED_BODY()
	
public:
    UBTS_CheckTargetState();

protected:
    UPROPERTY(EditAnywhere, Category = Blackboard)
    FBlackboardKeySelector TargetKey;

    UPROPERTY(EditAnywhere, Category = Blackboard)
    FBlackboardKeySelector TargetStateKey;

    virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
    void ClearTargetStateKeys(UBlackboardComponent* BlackboardComp);
};
