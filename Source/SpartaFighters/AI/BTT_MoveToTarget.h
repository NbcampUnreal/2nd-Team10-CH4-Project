#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "Navigation/PathFollowingComponent.h"
#include "BTT_MoveToTarget.generated.h"

UCLASS()
class SPARTAFIGHTERS_API UBTT_MoveToTarget : public UBTTaskNode
{
    GENERATED_BODY()

public:
    UBTT_MoveToTarget();

    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
    virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

protected:
    UPROPERTY(EditAnywhere, Category = "AI")
    FBlackboardKeySelector TargetKey;

    UPROPERTY(EditAnywhere, Category = "AI")
    float AcceptanceRadius;

private:
    TWeakObjectPtr<APawn> AIPawn;

    TWeakObjectPtr<AActor> TargetActor;

    FTimerHandle TimerHandle_MoveTimeout;

    bool bTaskFinished = false;


    FVector Calculate8Direction(const FVector& CurrentLocation, const FVector& TargetLocation) const;

    FVector FindValidDirectionTowardsTarget(const FVector& CurrentLocation, const FVector& TargetLocation) const;

    bool IsPathValid(const FVector& Destination) const;

    void UpdateMovement(const FVector& Direction) const;
};
