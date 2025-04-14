


#include "BTT_SelectMeleeAction.h"
#include "AI/Character/Enum/AIActionState.h"
#include "AI/Character/Enum/AIState.h"
#include "Character/Components/StateComponent.h"
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
		UE_LOG(LogTemp, Warning, TEXT("UBTT_SelectMeleeAction: BlackboardComponent is missing."));
		return EBTNodeResult::Failed;
	}

	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController)
	{
		UE_LOG(LogTemp, Warning, TEXT("UBTT_SelectMeleeAction: AIController is missing."));
		return EBTNodeResult::Failed;
	}

    // 블랙보드 키 이름이 유효한지 확인 (에디터에서 설정되었는지)
    if (TargetStateKey.SelectedKeyName == NAME_None || AIActionKey.SelectedKeyName == NAME_None)
    {
        UE_LOG(LogTemp, Warning, TEXT("BTT_SelectMeleeAction: Blackboard keys not set in Behavior Tree!"));
        return EBTNodeResult::Failed;
    }

    // 1. 블랙보드에서 타겟의 상태 읽기 (Enum 값은 uint8로 저장됨)
    uint8 TargetStateUint8 = BlackboardComp->GetValueAsEnum(TargetStateKey.SelectedKeyName);
    ECharacterState TargetState = static_cast<ECharacterState>(TargetStateUint8);

    // 기본적으로 선택될 액션 (초기값)
    EAIActionType SelectedAction = EAIActionType::None;

    // 2. 타겟 상태에 따라 AI 행동 결정 (게임 디자인에 맞춰 로직 구현)
    switch (TargetState)
    {
    case ECharacterState::Idle:
        // 타겟이 가만히 있으면 기본 공격 시도
        SelectedAction = EAIActionType::Attack;
        UE_LOG(LogTemp, Log, TEXT("BTT_SelectMeleeAction: Target is Idle. Selecting BasicAttack."));
        break;

    case ECharacterState::Moving:
        // 타겟이 움직이면 거리를 좁히거나 예측 공격 시도 (여기서는 예시로 Reposition)
        // 또는 거리에 따라 다른 판단 가능 (TargetActorKey 필요)
        SelectedAction = EAIActionType::Chase; // 또는 BasicAttack 등
        UE_LOG(LogTemp, Log, TEXT("BTT_SelectMeleeAction: Target is Moving. Selecting Reposition."));
        break;

    case ECharacterState::Crouching:
        // 타겟이 앉아있으면 느리므로 강 공격 기회?
        SelectedAction = EAIActionType::Attack;
        UE_LOG(LogTemp, Log, TEXT("BTT_SelectMeleeAction: Target is Crouching. Selecting HeavyAttack."));
        break;

    case ECharacterState::InAir:
        // 타겟이 공중에 있으면 공격이 어려울 수 있음. 회피하거나 착지 지점 예측?
        SelectedAction = EAIActionType::None; // 또는 Reposition, None 등
        UE_LOG(LogTemp, Log, TEXT("BTT_SelectMeleeAction: Target is InAir. Selecting Dodge."));
        break;

        // 만약 StateComponent에 Attack 같은 상태가 있다면 추가
        // case ECharacterState::Attacking:
        //     // 타겟이 공격 중이면 방어나 회피
        //     SelectedAction = EAIMeleeAction::Block; // 또는 Dodge
        //     break;

    default:
        // 알 수 없는 상태이거나 처리할 필요 없는 상태
        UE_LOG(LogTemp, Warning, TEXT("BTT_SelectMeleeAction: Unhandled Target State (%d). Selecting None."), TargetStateUint8);
        SelectedAction = EAIActionType::None;
        break;
    }

    // 3. 결정된 행동을 블랙보드의 AIActionKey에 저장 (Enum 값은 uint8로 저장)
    if (SelectedAction != EAIActionType::None)
    {
        BlackboardComp->SetValueAsEnum(AIActionKey.SelectedKeyName, static_cast<uint8>(SelectedAction));
        // 성공적으로 액션을 결정했으므로 Succeeded 반환
        return EBTNodeResult::Succeeded;
    }
    else
    {
        // 적절한 액션을 결정하지 못했으면 Failed 반환
        // 또는 상황에 따라 다른 기본 액션을 설정하고 Succeeded를 반환할 수도 있음 (예: 항상 BasicAttack 시도)
        BlackboardComp->ClearValue(AIActionKey.SelectedKeyName); // 액션 키 클리어
        return EBTNodeResult::Failed;
    }








	// TODO : Change actions depending on the status of the opposing player

	//int32 RandomIndex = FMath::RandRange(1, 4);
	//EAIActionType MeleeAction = static_cast<EAIActionType>(RandomIndex);

	//BlackboardComp->SetValueAsEnum(AIActionKey.SelectedKeyName, static_cast<uint8>(MeleeAction));

	//UE_LOG(LogTemp, Log, TEXT("[Melee Action] Selected: %s"), *UEnum::GetValueAsString(MeleeAction));

	//return EBTNodeResult::Succeeded;
}
