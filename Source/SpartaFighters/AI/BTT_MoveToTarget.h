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
    /** Blackboard key for the target actor */
    UPROPERTY(EditAnywhere, Category = "AI")
    FBlackboardKeySelector TargetKey;

    /** How close is "close enough" */
    UPROPERTY(EditAnywhere, Category = "AI")
    float AcceptanceRadius;

private:
    /** Caching pointers */
    TWeakObjectPtr<APawn> AIPawn;
    TWeakObjectPtr<AActor> TargetActor;

    /** Timer for move timeout */
    FTimerHandle TimerHandle_MoveTimeout;
    bool bTaskFinished = false;

    /** Compute one of 8 cardinal/intercardinal directions toward target */
    FVector Calculate8Direction(const FVector& CurrentLocation, const FVector& TargetLocation) const;
    /** Find a valid direction among 8 that has a path on NavMesh */
    FVector FindValidDirectionTowardsTarget(const FVector& CurrentLocation, const FVector& TargetLocation) const;
    /** Check if the NavMesh has a path to Destination */
    bool IsPathValid(const FVector& Destination) const;
    /** Apply movement and rotation to pawn */
    void UpdateMovement(const FVector& Direction) const;
};
