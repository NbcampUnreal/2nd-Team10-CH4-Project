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

private:
    FVector Calculate8Direction(const FVector& CurrentLocation, const FVector& TargetLocation) const;

    void UpdateMovement(const FVector& Direction) const;

    UPROPERTY()
    AActor* TargetCharacter = nullptr;

    UPROPERTY()
    APawn* AIPawn = nullptr;

    UPROPERTY(EditAnywhere, Category = "Blackboard")
    struct FBlackboardKeySelector TargetKey;

    UPROPERTY(EditAnywhere, Category = "Movement")
    float AcceptanceRadius;

    FTimerHandle TimerHandle_MoveTimeout;
    bool bTaskFinished = false;
};
