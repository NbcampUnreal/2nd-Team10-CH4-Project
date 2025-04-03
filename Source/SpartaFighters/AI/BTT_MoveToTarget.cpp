


#include "AI/BTT_MoveToTarget.h"
#include "AI/AICharacterController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Actor.h"

UBTT_MoveToTarget::UBTT_MoveToTarget()
{
	NodeName = "Move To Target";
	AcceptanceRadius = 200.0f;
	bNotifyTaskFinished = true;
	// each AI created
	bCreateNodeInstance = true;

	bTaskFinished = false;
}

EBTNodeResult::Type UBTT_MoveToTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	BTComponent = &OwnerComp;
	bTaskFinished = false;

	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController)
	{
		return EBTNodeResult::Failed;
	}

	if (AIController->ReceiveMoveCompleted.IsAlreadyBound(this, &UBTT_MoveToTarget::OnMoveCompleted))
	{
		AIController->ReceiveMoveCompleted.RemoveDynamic(this, &UBTT_MoveToTarget::OnMoveCompleted);
	}

	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (!BlackboardComp)
	{
		return EBTNodeResult::Failed;
	}

	AActor* TargetActor = Cast<AActor>(BlackboardComp->GetValueAsObject(TargetKey.SelectedKeyName));
	if (!TargetActor)
	{
		return EBTNodeResult::Failed;
	}

	GetWorld()->GetTimerManager().SetTimer(TimerHandle_MoveTimeout, [this, &OwnerComp]()
		{
			if (!bTaskFinished)
			{
				UE_LOG(LogTemp, Warning, TEXT("Move timed out!"));
				FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
			}
		}, 3.0f, false);

	float Distance = FVector::Dist(AIController->GetPawn()->GetActorLocation(), TargetActor->GetActorLocation());
	if (Distance <= AcceptanceRadius)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return EBTNodeResult::Succeeded;
	}

	FAIRequestID RequestID = AIController->MoveToActor(TargetActor, AcceptanceRadius, true, true, true);
	if (RequestID.IsValid())
	{
		AIController->ReceiveMoveCompleted.AddDynamic(this, &UBTT_MoveToTarget::OnMoveCompleted);
		return EBTNodeResult::InProgress;
	}

	return EBTNodeResult::Failed;
}

void UBTT_MoveToTarget::OnMoveCompleted(FAIRequestID RequestID, EPathFollowingResult::Type Result)
{
	if (bTaskFinished)
	{
		return;
	}
	bTaskFinished = true;

	if (BTComponent)
	{
		AAIController* AIController = BTComponent->GetAIOwner();
		if (AIController)
		{
			AIController->ReceiveMoveCompleted.RemoveDynamic(this, &UBTT_MoveToTarget::OnMoveCompleted);
		}
		if (Result == EPathFollowingResult::Success)
		{
			FinishLatentTask(*BTComponent, EBTNodeResult::Succeeded); 
		}
		else
		{
			FinishLatentTask(*BTComponent, EBTNodeResult::Failed);
		}
	}
}
