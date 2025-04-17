


#include "BTT_SelectRangeAction.h"
#include "Character/Components/StateComponent.h"
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
        UE_LOG(LogTemp, Warning, TEXT("UBTT_SelectMeleeAction: BlackboardComponent is missing."));
        return EBTNodeResult::Failed;
    }

    AAIController* AIController = OwnerComp.GetAIOwner();
    if (!AIController)
    {
        UE_LOG(LogTemp, Warning, TEXT("UBTT_SelectMeleeAction: AIController is missing."));
        return EBTNodeResult::Failed;
    }

    if (TargetStateKey.SelectedKeyName == NAME_None || AIActionKey.SelectedKeyName == NAME_None)
    {
        UE_LOG(LogTemp, Warning, TEXT("BTT_SelectMeleeAction: Blackboard keys not set in Behavior Tree!"));
        return EBTNodeResult::Failed;
    }

    uint8 TargetStateUint8 = BlackboardComp->GetValueAsEnum(TargetStateKey.SelectedKeyName);
    ECharacterState TargetState = static_cast<ECharacterState>(TargetStateUint8);

    EAIActionType SelectedAction = EAIActionType::Attack;
    EAIState SelectedState = EAIState::Idle;

    switch (TargetState)
    {
    case ECharacterState::Idle:
        SelectActionForIdleTarget(SelectedAction, SelectedState);
        break;

    case ECharacterState::Moving:
        SelectActionForMoveTarget(SelectedAction, SelectedState);
        break;

    case ECharacterState::Crouching:
        SelectActionForCrouchTarget(SelectedAction, SelectedState);
        break;

    case ECharacterState::InAir:
        SelectActionForJumpTarget(SelectedAction, SelectedState);
        break;

        // case ECharacterState::Attacking:
        //     // 타겟이 공격 중이면 방어나 회피
        //     SelectedAction = EAIMeleeAction::Block; // 또는 Dodge
        //     break;

    default:
        UE_LOG(LogTemp, Warning, TEXT("BTT_SelectMeleeAction: Unhandled Target State (%d). Selecting None."), TargetStateUint8);
        SelectedAction = EAIActionType::None;
        SelectedState = EAIState::None;
        break;
    }

    if (SelectedAction != EAIActionType::None)
    {
        BlackboardComp->SetValueAsEnum(AIActionKey.SelectedKeyName, static_cast<uint8>(SelectedAction));
        BlackboardComp->SetValueAsEnum(AIStateKey.SelectedKeyName, static_cast<uint8>(SelectedState));
        return EBTNodeResult::Succeeded;
    }
    else
    {
        BlackboardComp->ClearValue(AIActionKey.SelectedKeyName);
        BlackboardComp->ClearValue(AIStateKey.SelectedKeyName);
        return EBTNodeResult::Failed;
    }
}

void UBTT_SelectRangeAction::SelectActionForIdleTarget(EAIActionType& NewAction, EAIState& NewState)
{
    const int32 BasicAttack = 10;
    const int32 CrounchAttack = 10;
    const int32 JumpAttack = 10;
    const int32 Evade = 10;
    const int32 Retreat = 60;
    const int32 TotalValue = BasicAttack + CrounchAttack + JumpAttack + Evade + Retreat;

    int32 RandomValue = FMath::RandRange(0, TotalValue - 1);

    if (RandomValue < BasicAttack)
    {
        NewState = EAIState::Idle;
        if (FMath::RandRange(0, 99) < 70)
        {
            NewAction = EAIActionType::Attack;
        }
        else
        {
            NewAction = EAIActionType::SpecialAttack;
        }
    }
    else if (RandomValue < BasicAttack + CrounchAttack)
    {
        NewState = EAIState::Jump;
        if (FMath::RandRange(0, 1) == 0)
        {
            NewAction = EAIActionType::Attack;
        }
        else
        {
            NewAction = EAIActionType::SpecialAttack;
        }
    }
    else if (RandomValue < BasicAttack + CrounchAttack + JumpAttack)
    {
        NewState = EAIState::Crouch;
        if (FMath::RandRange(0, 1) == 0)
        {
            NewAction = EAIActionType::Attack;
        }
        else
        {
            NewAction = EAIActionType::SpecialAttack;
        }
    }
    else if (RandomValue < BasicAttack + CrounchAttack + JumpAttack)
    {
        NewState = EAIState::Idle;
        NewAction = EAIActionType::Evade;
    }
    else
    {
        NewState = EAIState::Idle;
        NewAction = EAIActionType::Retreat;
    }
}

void UBTT_SelectRangeAction::SelectActionForCrouchTarget(EAIActionType& NewAction, EAIState& NewState)
{
    const int32 BasicAttack = 10;
    const int32 CrounchAttack = 10;
    const int32 JumpAttack = 10;
    const int32 Evade = 10;
    const int32 Retreat = 60;
    const int32 TotalValue = BasicAttack + CrounchAttack + JumpAttack + Evade + Retreat;

    int32 RandomValue = FMath::RandRange(0, TotalValue - 1);

    if (RandomValue < BasicAttack)
    {
        NewState = EAIState::Idle;
        if (FMath::RandRange(0, 99) < 70)
        {
            NewAction = EAIActionType::Attack;
        }
        else
        {
            NewAction = EAIActionType::SpecialAttack;
        }
    }
    else if (RandomValue < BasicAttack + CrounchAttack)
    {
        NewState = EAIState::Crouch;
        if (FMath::RandRange(0, 1) == 0)
        {
            NewAction = EAIActionType::Attack;
        }
        else
        {
            NewAction = EAIActionType::SpecialAttack;
        }
    }
    else if (RandomValue < BasicAttack + CrounchAttack + JumpAttack)
    {
        NewState = EAIState::Jump;
        if (FMath::RandRange(0, 1) == 0)
        {
            NewAction = EAIActionType::Attack;
        }
        else
        {
            NewAction = EAIActionType::SpecialAttack;
        }
    }
    else if (RandomValue < BasicAttack + CrounchAttack + JumpAttack)
    {
        NewState = EAIState::Idle;
        NewAction = EAIActionType::Evade;
    }
    else
    {
        NewState = EAIState::Idle;
        NewAction = EAIActionType::Retreat;
    }
}

void UBTT_SelectRangeAction::SelectActionForJumpTarget(EAIActionType& NewAction, EAIState& NewState)
{
    const int32 BasicAttack = 10;
    const int32 CrounchAttack = 10;
    const int32 JumpAttack = 10;
    const int32 Evade = 10;
    const int32 Retreat = 60;
    const int32 TotalValue = BasicAttack + CrounchAttack + JumpAttack + Evade + Retreat;

    int32 RandomValue = FMath::RandRange(0, TotalValue - 1);

    if (RandomValue < BasicAttack)
    {
        NewState = EAIState::Idle;
        if (FMath::RandRange(0, 99) < 70)
        {
            NewAction = EAIActionType::Attack;
        }
        else
        {
            NewAction = EAIActionType::SpecialAttack;
        }
    }
    else if (RandomValue < BasicAttack + CrounchAttack)
    {
        NewState = EAIState::Crouch;
        NewAction = EAIActionType::SpecialAttack;

    }
    else if (RandomValue < BasicAttack + CrounchAttack + JumpAttack)
    {
        NewState = EAIState::Jump;
        NewAction = EAIActionType::SpecialAttack;
    }
    else if (RandomValue < BasicAttack + CrounchAttack + JumpAttack)
    {
        NewState = EAIState::Idle;
        NewAction = EAIActionType::Evade;
    }
    else
    {
        NewState = EAIState::Idle;
        NewAction = EAIActionType::Retreat;
    }
}

void UBTT_SelectRangeAction::SelectActionForMoveTarget(EAIActionType& NewAction, EAIState& NewState)
{
    const int32 BasicAttack = 10;
    const int32 CrounchAttack = 10;
    const int32 JumpAttack = 10;
    const int32 Evade = 10;
    const int32 Retreat = 60;
    const int32 TotalValue = BasicAttack + CrounchAttack + JumpAttack + Evade + Retreat;

    int32 RandomValue = FMath::RandRange(0, TotalValue - 1);

    if (RandomValue < BasicAttack)
    {
        NewState = EAIState::Idle;
        if (FMath::RandRange(0, 99) < 70)
        {
            NewAction = EAIActionType::Attack;
        }
        else
        {
            NewAction = EAIActionType::SpecialAttack;
        }
    }
    else if (RandomValue < BasicAttack + CrounchAttack)
    {
        NewState = EAIState::Crouch;
        if (FMath::RandRange(0, 1) == 0)
        {
            NewAction = EAIActionType::Attack;
        }
        else
        {
            NewAction = EAIActionType::SpecialAttack;
        }
    }
    else if (RandomValue < BasicAttack + CrounchAttack + JumpAttack)
    {
        NewState = EAIState::Jump;
        if (FMath::RandRange(0, 1) == 0)
        {
            NewAction = EAIActionType::Attack;
        }
        else
        {
            NewAction = EAIActionType::SpecialAttack;
        }
    }
    else if (RandomValue < BasicAttack + CrounchAttack + JumpAttack)
    {
        NewState = EAIState::Idle;
        NewAction = EAIActionType::Evade;
    }
    else
    {
        NewState = EAIState::Idle;
        NewAction = EAIActionType::Retreat;
    }
}

void UBTT_SelectRangeAction::SelectActionForGuardTarget(EAIActionType& NewAction, EAIState& NewState)
{
    const int32 BasicAttack = 10;
    const int32 CrounchAttack = 10;
    const int32 JumpAttack = 10;
    const int32 Evade = 10;
    const int32 Retreat = 60;
    const int32 TotalValue = BasicAttack + CrounchAttack + JumpAttack + Evade + Retreat;

    int32 RandomValue = FMath::RandRange(0, TotalValue - 1);

    if (RandomValue < BasicAttack)
    {
        NewState = EAIState::Idle;
        if (FMath::RandRange(0, 99) < 70)
        {
            NewAction = EAIActionType::Attack;
        }
        else
        {
            NewAction = EAIActionType::SpecialAttack;
        }
    }
    else if (RandomValue < BasicAttack + CrounchAttack)
    {
        NewState = EAIState::Crouch;
        if (FMath::RandRange(0, 1) == 0)
        {
            NewAction = EAIActionType::Attack;
        }
        else
        {
            NewAction = EAIActionType::SpecialAttack;
        }
    }
    else if (RandomValue < BasicAttack + CrounchAttack + JumpAttack)
    {
        NewState = EAIState::Jump;
        if (FMath::RandRange(0, 1) == 0)
        {
            NewAction = EAIActionType::Attack;
        }
        else
        {
            NewAction = EAIActionType::SpecialAttack;
        }
    }
    else if (RandomValue < BasicAttack + CrounchAttack + JumpAttack)
    {
        NewState = EAIState::Idle;
        NewAction = EAIActionType::Evade;
    }
    else
    {
        NewState = EAIState::Idle;
        NewAction = EAIActionType::Retreat;
    }
}

void UBTT_SelectRangeAction::SelectActionForAttackTarget(EAIActionType& NewAction, EAIState& NewState)
{
    const int32 BasicAttack = 10;
    const int32 CrounchAttack = 10;
    const int32 JumpAttack = 10;
    const int32 Evade = 10;
    const int32 Retreat = 60;
    const int32 TotalValue = BasicAttack + CrounchAttack + JumpAttack + Evade + Retreat;

    int32 RandomValue = FMath::RandRange(0, TotalValue - 1);

    if (RandomValue < BasicAttack)
    {
        NewState = EAIState::Idle;
        if (FMath::RandRange(0, 99) < 70)
        {
            NewAction = EAIActionType::Attack;
        }
        else
        {
            NewAction = EAIActionType::SpecialAttack;
        }
    }
    else if (RandomValue < BasicAttack + CrounchAttack)
    {
        NewState = EAIState::Crouch;
        if (FMath::RandRange(0, 1) == 0)
        {
            NewAction = EAIActionType::Attack;
        }
        else
        {
            NewAction = EAIActionType::SpecialAttack;
        }
    }
    else if (RandomValue < BasicAttack + CrounchAttack + JumpAttack)
    {
        NewState = EAIState::Jump;
        if (FMath::RandRange(0, 1) == 0)
        {
            NewAction = EAIActionType::Attack;
        }
        else
        {
            NewAction = EAIActionType::SpecialAttack;
        }
    }
    else if (RandomValue < BasicAttack + CrounchAttack + JumpAttack)
    {
        NewState = EAIState::Idle;
        NewAction = EAIActionType::Evade;
    }
    else
    {
        NewState = EAIState::Idle;
        NewAction = EAIActionType::Retreat;
    }
}

void UBTT_SelectRangeAction::SelectActionForRollTarget(EAIActionType& NewAction, EAIState& NewState)
{
    const int32 BasicAttack = 10;
    const int32 CrounchAttack = 10;
    const int32 JumpAttack = 10;
    const int32 Evade = 10;
    const int32 Retreat = 60;
    const int32 TotalValue = BasicAttack + CrounchAttack + JumpAttack + Evade + Retreat;

    int32 RandomValue = FMath::RandRange(0, TotalValue - 1);

    if (RandomValue < BasicAttack)
    {
        NewState = EAIState::Idle;
        if (FMath::RandRange(0, 99) < 70)
        {
            NewAction = EAIActionType::Attack;
        }
        else
        {
            NewAction = EAIActionType::SpecialAttack;
        }
    }
    else if (RandomValue < BasicAttack + CrounchAttack)
    {
        NewState = EAIState::Crouch;
        if (FMath::RandRange(0, 1) == 0)
        {
            NewAction = EAIActionType::Attack;
        }
        else
        {
            NewAction = EAIActionType::SpecialAttack;
        }
    }
    else if (RandomValue < BasicAttack + CrounchAttack + JumpAttack)
    {
        NewState = EAIState::Jump;
        if (FMath::RandRange(0, 1) == 0)
        {
            NewAction = EAIActionType::Attack;
        }
        else
        {
            NewAction = EAIActionType::SpecialAttack;
        }
    }
    else if (RandomValue < BasicAttack + CrounchAttack + JumpAttack)
    {
        NewState = EAIState::Idle;
        NewAction = EAIActionType::Evade;
    }
    else
    {
        NewState = EAIState::Idle;
        NewAction = EAIActionType::Retreat;
    }
}
