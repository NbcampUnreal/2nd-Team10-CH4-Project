

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_ExecuteAIAction.generated.h"

// After the state is introduced, it needs to be completely overhauled.

UCLASS()
class SPARTAFIGHTERS_API UBTT_ExecuteAIAction : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
    UBTT_ExecuteAIAction();

protected:
    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
    UFUNCTION()
    void OnMontageEnded(UAnimMontage* Montage, bool bInterrupted);

    UFUNCTION()
    void OnMontageBlendingOut(UAnimMontage* Montage, bool bInterrupted);

    void FinishTask(UBehaviorTreeComponent* OwnerComp, EBTNodeResult::Type Result);

    UPROPERTY(EditAnywhere, Category = "Animation")
    UAnimMontage* MontageToPlay;

    FOnMontageEnded MontageEndDelegate;

    FOnMontageBlendingOutStarted BlendingOutDelegate;

    UBehaviorTreeComponent* CachedOwnerComp;

    FTimerHandle TimerHandle;
};
