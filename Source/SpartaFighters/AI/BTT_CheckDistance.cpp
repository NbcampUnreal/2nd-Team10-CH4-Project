#include "BTT_CheckDistance.h"
#include "AI/Character/Enum/DistanceState.h"
#include "GameFramework/Actor.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"

UBTT_CheckDistance::UBTT_CheckDistance()
{
	NodeName = TEXT("Check Distance");

	bNotifyTick = false;
	MeleeRange = 160.f;
	RangedRange = 600.f;
}

EBTNodeResult::Type UBTT_CheckDistance::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController)
	{
		return EBTNodeResult::Failed;
	}
	
	APawn* AIPawn = AIController->GetPawn();
	if (!AIPawn)
	{
		return EBTNodeResult::Failed;
	}

	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
	if (!Blackboard)
	{
		return EBTNodeResult::Failed;
	}

	AActor* TargetActor = Cast<AActor>(Blackboard->GetValueAsObject(TargetKey.SelectedKeyName));
	if (!TargetActor)
	{
		return EBTNodeResult::Failed;
	}

	float Distance = FVector::Dist(AIPawn->GetActorLocation(), TargetActor->GetActorLocation());

	EDistanceType Category = EDistanceType::None;

	if (Distance <= MeleeRange)
	{
		Category = EDistanceType::Melee;
	}
	else if (Distance <= RangedRange)
	{
		Category = EDistanceType::Ranged;
	}
	else
	{
		Category = EDistanceType::TooFar;
	}

	Blackboard->SetValueAsEnum(DistanceTypeKey.SelectedKeyName, static_cast<uint8>(Category));

	return EBTNodeResult::Succeeded;
}