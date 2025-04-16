


#include "BTT_RandomTypeAttack.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"

UBTT_RandomTypeAttack::UBTT_RandomTypeAttack()
{
    NodeName = TEXT("Random Type Attack");
}

EBTNodeResult::Type UBTT_RandomTypeAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (!BlackboardComp)
	{
		return EBTNodeResult::Failed;
	}

	int32 RandomValue = FMath::RandRange(0, 2);

	BlackboardComp->SetValueAsInt(AttackTypeKey.SelectedKeyName, RandomValue);

	return EBTNodeResult::Succeeded;
}
