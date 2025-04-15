

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_RotateToTarget.generated.h"

/**
 * 
 */
UCLASS()
class SPARTAFIGHTERS_API UBTT_RotateToTarget : public UBTTaskNode
{
	GENERATED_BODY()

public:
    UBTT_RotateToTarget();

protected:
    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	UPROPERTY(EditAnywhere, Category = "Blackboard")
    struct FBlackboardKeySelector TargetKey;

private:
	UPROPERTY()
	APawn* AIPawn;

	UPROPERTY()
	AActor* TargetActor;

	UPROPERTY(EditAnywhere, Category = "Rotation")
	float RotationInterpSpeed = 30.0f;

	UPROPERTY(EditAnywhere, Category = "Rotation")
	float AcceptableAngle = 5.0f; // 몇 도 이내면 회전 완료로 간주


};
