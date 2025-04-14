


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

    // ������ Ű �̸��� ��ȿ���� Ȯ�� (�����Ϳ��� �����Ǿ�����)
    if (TargetStateKey.SelectedKeyName == NAME_None || AIActionKey.SelectedKeyName == NAME_None)
    {
        UE_LOG(LogTemp, Warning, TEXT("BTT_SelectMeleeAction: Blackboard keys not set in Behavior Tree!"));
        return EBTNodeResult::Failed;
    }

    // 1. �����忡�� Ÿ���� ���� �б� (Enum ���� uint8�� �����)
    uint8 TargetStateUint8 = BlackboardComp->GetValueAsEnum(TargetStateKey.SelectedKeyName);
    ECharacterState TargetState = static_cast<ECharacterState>(TargetStateUint8);

    // �⺻������ ���õ� �׼� (�ʱⰪ)
    EAIActionType SelectedAction = EAIActionType::None;

    // 2. Ÿ�� ���¿� ���� AI �ൿ ���� (���� �����ο� ���� ���� ����)
    switch (TargetState)
    {
    case ECharacterState::Idle:
        // Ÿ���� ������ ������ �⺻ ���� �õ�
        SelectedAction = EAIActionType::Attack;
        UE_LOG(LogTemp, Log, TEXT("BTT_SelectMeleeAction: Target is Idle. Selecting BasicAttack."));
        break;

    case ECharacterState::Moving:
        // Ÿ���� �����̸� �Ÿ��� �����ų� ���� ���� �õ� (���⼭�� ���÷� Reposition)
        // �Ǵ� �Ÿ��� ���� �ٸ� �Ǵ� ���� (TargetActorKey �ʿ�)
        SelectedAction = EAIActionType::Chase; // �Ǵ� BasicAttack ��
        UE_LOG(LogTemp, Log, TEXT("BTT_SelectMeleeAction: Target is Moving. Selecting Reposition."));
        break;

    case ECharacterState::Crouching:
        // Ÿ���� �ɾ������� �����Ƿ� �� ���� ��ȸ?
        SelectedAction = EAIActionType::Attack;
        UE_LOG(LogTemp, Log, TEXT("BTT_SelectMeleeAction: Target is Crouching. Selecting HeavyAttack."));
        break;

    case ECharacterState::InAir:
        // Ÿ���� ���߿� ������ ������ ����� �� ����. ȸ���ϰų� ���� ���� ����?
        SelectedAction = EAIActionType::None; // �Ǵ� Reposition, None ��
        UE_LOG(LogTemp, Log, TEXT("BTT_SelectMeleeAction: Target is InAir. Selecting Dodge."));
        break;

        // ���� StateComponent�� Attack ���� ���°� �ִٸ� �߰�
        // case ECharacterState::Attacking:
        //     // Ÿ���� ���� ���̸� �� ȸ��
        //     SelectedAction = EAIMeleeAction::Block; // �Ǵ� Dodge
        //     break;

    default:
        // �� �� ���� �����̰ų� ó���� �ʿ� ���� ����
        UE_LOG(LogTemp, Warning, TEXT("BTT_SelectMeleeAction: Unhandled Target State (%d). Selecting None."), TargetStateUint8);
        SelectedAction = EAIActionType::None;
        break;
    }

    // 3. ������ �ൿ�� �������� AIActionKey�� ���� (Enum ���� uint8�� ����)
    if (SelectedAction != EAIActionType::None)
    {
        BlackboardComp->SetValueAsEnum(AIActionKey.SelectedKeyName, static_cast<uint8>(SelectedAction));
        // ���������� �׼��� ���������Ƿ� Succeeded ��ȯ
        return EBTNodeResult::Succeeded;
    }
    else
    {
        // ������ �׼��� �������� �������� Failed ��ȯ
        // �Ǵ� ��Ȳ�� ���� �ٸ� �⺻ �׼��� �����ϰ� Succeeded�� ��ȯ�� ���� ���� (��: �׻� BasicAttack �õ�)
        BlackboardComp->ClearValue(AIActionKey.SelectedKeyName); // �׼� Ű Ŭ����
        return EBTNodeResult::Failed;
    }








	// TODO : Change actions depending on the status of the opposing player

	//int32 RandomIndex = FMath::RandRange(1, 4);
	//EAIActionType MeleeAction = static_cast<EAIActionType>(RandomIndex);

	//BlackboardComp->SetValueAsEnum(AIActionKey.SelectedKeyName, static_cast<uint8>(MeleeAction));

	//UE_LOG(LogTemp, Log, TEXT("[Melee Action] Selected: %s"), *UEnum::GetValueAsString(MeleeAction));

	//return EBTNodeResult::Succeeded;
}
