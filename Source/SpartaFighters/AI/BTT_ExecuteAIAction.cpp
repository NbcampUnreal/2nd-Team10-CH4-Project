#include "BTT_ExecuteAIAction.h"
#include "AI/AICharacterController.h"
#include "Character/SFCharacter.h"
#include "Character/Components/SkillComponent.h"
#include "AI/Character/Enum/AIActionState.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"
#include "Animation/AnimInstance.h"


UBTT_ExecuteAIAction::UBTT_ExecuteAIAction()
{
    NodeName = TEXT("Play Action");
}

EBTNodeResult::Type UBTT_ExecuteAIAction::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    AAIController* AIController = OwnerComp.GetAIOwner();
    if (!AIController)
    {
        UE_LOG(LogTemp, Error, TEXT("UBTT_ExecuteAIAction::ExecuteTask - AIController is Null"));
        return EBTNodeResult::Failed;
    }

    UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
    if (!BlackboardComp)
    {
        UE_LOG(LogTemp, Error, TEXT("UBTT_ExecuteAIAction::ExecuteTask - BlackboardComp is Null"));
        return EBTNodeResult::Failed;
    }

    if (AIActionKey.SelectedKeyName == NAME_None)
    {
        UE_LOG(LogTemp, Error, TEXT("UBTT_ExecuteAIAction::ExecuteTask - AIActionKey is not set in Behavior Tree!"));
        return EBTNodeResult::Failed;
    }

    ASFCharacter* SFCharacter = Cast<ASFCharacter>(AIController->GetPawn());
    if (!SFCharacter)
    {
        UE_LOG(LogTemp, Error, TEXT("UBTT_ExecuteAIAction::ExecuteTask - AI Pawn is not an SFCharacter"));
        return EBTNodeResult::Failed;
    }

    USkillComponent* SkillComp = SFCharacter->SkillComponent;
    if (!SkillComp)
    {
        UE_LOG(LogTemp, Error, TEXT("UBTT_ExecuteAIAction::ExecuteTask - SkillComponent is Null on %s"), *GetNameSafe(SFCharacter));
        return EBTNodeResult::Failed;
    }

    UStateComponent* StateComp = SFCharacter->StateComponent;
    if (!StateComp)
    {
        UE_LOG(LogTemp, Warning, TEXT("UBTT_ExecuteAIAction::ExecuteTask - StateComponent is Null on %s (State checks disabled)"), *GetNameSafe(SFCharacter));
    }


    if (StateComp && StateComp->IsInAction())
    {
        UE_LOG(LogTemp, Log, TEXT("UBTT_ExecuteAIAction::ExecuteTask - AI is already in action. Skipping new action."));
        return EBTNodeResult::Failed;
    }


    uint8 AIAction = BlackboardComp->GetValueAsEnum(AIActionKey.SelectedKeyName);
    EAIActionType SelectedAction = static_cast<EAIActionType>(AIAction);

    bool bActionStarted = false;

    switch (SelectedAction)
    {
    case EAIActionType::Attack:
        SkillComp->HandleInputBasicAttack();
        bActionStarted = true;
        break;

    case EAIActionType::SpecialAttack:
        SkillComp->HandleInputSkillAttack();
        bActionStarted = true;
        break;

    case EAIActionType::Guard:
        if (SkillComp->GetClass()->FindFunctionByName(FName("HandleInputGuard")))
        {

            bActionStarted = true;
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("UBTT_ExecuteAIAction: Guard action selected, but HandleInputGuard function not found/implemented in SkillComponent."));
        }
        break;

    case EAIActionType::Evade:
        SkillComp->HandleInputDodge();
        bActionStarted = true;
        break;

    case EAIActionType::Retreat:
        return EBTNodeResult::Failed;

    case EAIActionType::Chase:
        return EBTNodeResult::Failed;

    default:
        UE_LOG(LogTemp, Warning, TEXT("UBTT_ExecuteAIAction: Unhandled or Invalid AIActionType Enum (%d)."), AIAction);
        return EBTNodeResult::Failed;
    }

    if (bActionStarted)
    {
        return EBTNodeResult::Succeeded;
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("UBTT_ExecuteAIAction::ExecuteTask - Failed to start the selected action."));
        return EBTNodeResult::Failed;
    }
}