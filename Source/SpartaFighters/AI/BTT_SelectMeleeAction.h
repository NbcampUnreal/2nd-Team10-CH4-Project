

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "AI/Character/Enum/AIState.h"
#include "AI/Character/Enum/AIActionState.h"
#include "BTT_SelectMeleeAction.generated.h"

/**
 * 
 */
UCLASS()
class SPARTAFIGHTERS_API UBTT_SelectMeleeAction : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTT_SelectMeleeAction();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector AIActionKey;

	UPROPERTY(EditAnywhere, Category = Blackboard)
	FBlackboardKeySelector TargetStateKey;

	UPROPERTY(EditAnywhere, Category = Blackboard)
	FBlackboardKeySelector AIStateKey;

	void SelectActionForIdleTarget(EAIActionType& NewAction, EAIState& NewState);

	void SelectActionForCrouchTarget(EAIActionType& NewAction, EAIState& NewState);

	void SelectActionForJumpTarget(EAIActionType& NewAction, EAIState& NewState);

	void SelectActionForMoveTarget(EAIActionType& NewAction, EAIState& NewState);

	void SelectActionForGuardTarget(EAIActionType& NewAction, EAIState& NewState);

	void SelectActionForAttackTarget(EAIActionType& NewAction, EAIState& NewState);

	void SelectActionForRollTarget(EAIActionType& NewAction, EAIState& NewState);
};
