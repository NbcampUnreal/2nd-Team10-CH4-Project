


#include "BTT_SelectRangeAction.h"
#include "AI/Character/Enum/AIActionState.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTT_SelectRangeAction::UBTT_SelectRangeAction()
{
	NodeName = TEXT("Select Range Action");
}

EBTNodeResult::Type UBTT_SelectRangeAction::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (!BlackboardComp)
	{
		return EBTNodeResult::Failed;
	}

	// TODO : Change actions depending on the status of the opposing player

	int32 RandomIndex = FMath::RandRange(1, 5);

	// Temp ChaseAction
	RandomIndex = 5;

	EAIActionType RangeAction = static_cast<EAIActionType>(RandomIndex);

	BlackboardComp->SetValueAsEnum(AIActionKey.SelectedKeyName, static_cast<uint8>(RangeAction));

	UE_LOG(LogTemp, Log, TEXT("[Range Action] Selected: %s"), *UEnum::GetValueAsString(RangeAction));

	return EBTNodeResult::Succeeded;
}
