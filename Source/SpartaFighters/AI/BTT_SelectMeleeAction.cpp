


#include "BTT_SelectMeleeAction.h"
#include "AI/Character/Enum/AIActionState.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTT_SelectMeleeAction::UBTT_SelectMeleeAction()
{
	NodeName = TEXT("Select Melee Action");
}

EBTNodeResult::Type UBTT_SelectMeleeAction::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (!BlackboardComp)
	{
		return EBTNodeResult::Failed;
	}

	// TODO : Change actions depending on the status of the opposing player

	int32 RandomIndex = FMath::RandRange(1, 4);
	EAIActionType MeleeAction = static_cast<EAIActionType>(RandomIndex);

	BlackboardComp->SetValueAsEnum(AIActionKey.SelectedKeyName, static_cast<uint8>(MeleeAction));

	UE_LOG(LogTemp, Log, TEXT("[Melee Action] Selected: %s"), *UEnum::GetValueAsString(MeleeAction));

	return EBTNodeResult::Succeeded;
}
