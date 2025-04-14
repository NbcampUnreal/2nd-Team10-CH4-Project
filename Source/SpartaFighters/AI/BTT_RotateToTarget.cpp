


#include "BTT_RotateToTarget.h"
#include "AI/AICharacterController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTT_RotateToTarget::UBTT_RotateToTarget()
{
}

EBTNodeResult::Type UBTT_RotateToTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AIPawn = OwnerComp.GetAIOwner() ? OwnerComp.GetAIOwner()->GetPawn() : nullptr;
	if (!AIPawn) return EBTNodeResult::Failed;

	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (!BlackboardComp) return EBTNodeResult::Failed;

	TargetActor = Cast<AActor>(BlackboardComp->GetValueAsObject(TargetKey.SelectedKeyName));
	if (!TargetActor) return EBTNodeResult::Failed;

	bNotifyTick = true;
	return EBTNodeResult::InProgress;
}

void UBTT_RotateToTarget::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	if (!AIPawn || !TargetActor)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	FVector Direction = (TargetActor->GetActorLocation() - AIPawn->GetActorLocation()).GetSafeNormal2D();
	FRotator TargetRot = Direction.Rotation();
	FRotator CurrentRot = AIPawn->GetActorRotation();

	FRotator NewRot = FMath::RInterpTo(CurrentRot, TargetRot, DeltaSeconds, RotationInterpSpeed);
	AIPawn->SetActorRotation(NewRot);

	if (FMath::Abs(FRotator::NormalizeAxis(NewRot.Yaw - TargetRot.Yaw)) <= AcceptableAngle)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}
