


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

    UE_LOG(LogTemp, Log, TEXT("UBTT_ExecuteAIAction::ExecuteTask - Attempting Action: %s"), *UEnum::GetValueAsString(SelectedAction));

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

            UE_LOG(LogTemp, Warning, TEXT("UBTT_ExecuteAIAction: Guard action selected, assuming HandleInputGuard exists in SkillComponent."));
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
        UE_LOG(LogTemp, Log, TEXT("UBTT_ExecuteAIAction: Retreat action is navigational, should be handled by MoveTo tasks. Failing this task."));
        return EBTNodeResult::Failed;

    case EAIActionType::Chase:
        UE_LOG(LogTemp, Log, TEXT("UBTT_ExecuteAIAction: Chase action is navigational, should be handled by MoveTo tasks. Failing this task."));
        return EBTNodeResult::Failed;

    default:
        UE_LOG(LogTemp, Warning, TEXT("UBTT_ExecuteAIAction: Unhandled or Invalid AIActionType Enum (%d)."), AIAction);
        return EBTNodeResult::Failed;
    }

    if (bActionStarted)
    {
        UE_LOG(LogTemp, Log, TEXT("UBTT_ExecuteAIAction::ExecuteTask - Action request sent successfully. Returning Succeeded."));
        return EBTNodeResult::Succeeded;
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("UBTT_ExecuteAIAction::ExecuteTask - Failed to start the selected action."));
        return EBTNodeResult::Failed;
    }








    //UE_LOG(LogTemp, Log, TEXT("UBTT_ExecuteAIAction::ExecuteTask - Start"));

    //CachedOwnerComp = &OwnerComp;

    //AAIController* AIController = OwnerComp.GetAIOwner();
    //if (!AIController)
    //{
    //    UE_LOG(LogTemp, Error, TEXT("UBTT_ExecuteAIAction::ExecuteTask - AIController is Null"));
    //    return EBTNodeResult::Failed;
    //}

    //UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
    //if (!BlackboardComp)
    //{
    //    UE_LOG(LogTemp, Error, TEXT("UBTT_ExecuteAIAction::ExecuteTask - BlackboardComp is Null"));
    //    return EBTNodeResult::Failed;
    //}

    //ACharacter* AICharacter = Cast<ACharacter>(AIController->GetPawn());
    //if (!AICharacter)
    //{
    //    UE_LOG(LogTemp, Error, TEXT("UBTT_ExecuteAIAction::ExecuteTask - AICharacter is Null"));
    //    return EBTNodeResult::Failed;
    //}

    //UAnimInstance* AnimInstance = AICharacter->GetMesh()->GetAnimInstance();
    //if (!AnimInstance)
    //{
    //    UE_LOG(LogTemp, Error, TEXT("UBTT_ExecuteAIAction::ExecuteTask - AnimInstance is Null"));
    //    return EBTNodeResult::Failed;
    //}

    //ASFCharacter* SFCharacter = Cast<ASFCharacter>(AICharacter);
    //if (!SFCharacter)
    //{
    //    UE_LOG(LogTemp, Error, TEXT("UBTT_ExecuteAIAction::ExecuteTask - Cast to ASFCharacter failed"));
    //    return EBTNodeResult::Failed;
    //}

    //uint8 AIAction = BlackboardComp->GetValueAsEnum(AIActionKey.SelectedKeyName);

    //EAIActionType AIActionType = static_cast<EAIActionType>(AIAction);

    //switch (AIActionType)
    //{
    //case EAIActionType::Attack:
    //    SFCharacter->SpawnFireBall();
    //    break;

    //case EAIActionType::Guard:
    //    SFCharacter->SpawnFireBall();
    //    break;

    //case EAIActionType::Evade:
    //    SFCharacter->DodgePressed();
    //    break;

    //case EAIActionType::Retreat:
    //    SFCharacter->SpawnFireBall();
    //    break;
    //    
    //case EAIActionType::Chase:
    //    return EBTNodeResult::Failed;

    //default:
    //    UE_LOG(LogTemp, Warning, TEXT("Invalid AIAction Enum."));
    //    return EBTNodeResult::Failed;
    //}

    //return EBTNodeResult::Succeeded;

    //if (!MontageToPlay)
    //{
    //    UE_LOG(LogTemp, Error, TEXT("UBTT_ExecuteAIAction::ExecuteTask - MontageToPlay is Null"));
    //    return EBTNodeResult::Failed;
    //}

    // 몽타주 종료 이벤트 바인딩 (이젠 필요가 없을지도...)
    //// End Delegate 등록
    //MontageEndDelegate.BindUObject(this, &UBTT_ExecuteAIAction::OnMontageEnded);
    //AnimInstance->Montage_SetEndDelegate(MontageEndDelegate, MontageToPlay);

    //// Blending Out Delegate 등록
    //BlendingOutDelegate.BindUObject(this, &UBTT_ExecuteAIAction::OnMontageBlendingOut);
    //AnimInstance->Montage_SetBlendingOutDelegate(BlendingOutDelegate, MontageToPlay);
    //UE_LOG(LogTemp, Log, TEXT("UBTT_ExecuteAIAction::ExecuteTask - MontageEndDelegate Bind Complete"));

    //GetWorld()->GetTimerManager().SetTimer(
    //    TimerHandle,
    //    [this, AICharacter, AnimInstance]() {
    //        if (CachedOwnerComp && !AnimInstance->Montage_IsPlaying(MontageToPlay)) {
    //            UE_LOG(LogTemp, Log, TEXT("UBTT_ExecuteAIAction - Montage stopped playing, finishing task"));
    //            FinishLatentTask(*CachedOwnerComp, EBTNodeResult::Succeeded);
    //            GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
    //        }
    //    },
    //    0.1f,
    //    true
    //);

    //// 몽타주 재생
    //float PlayRate = AnimInstance->Montage_Play(MontageToPlay);
    //if (PlayRate <= 0.f)
    //{
    //    UE_LOG(LogTemp, Error, TEXT("UBTT_ExecuteAIAction::ExecuteTask - Montage dont play"));
    //    return EBTNodeResult::Failed;
    //}
    //UE_LOG(LogTemp, Log, TEXT("UBTT_ExecuteAIAction::ExecuteTask - Montage play, PlayRate: %f"), PlayRate);

    //return EBTNodeResult::InProgress;
}

void UBTT_ExecuteAIAction::OnMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
    UE_LOG(LogTemp, Log, TEXT("UBTT_ExecuteAIAction::OnMontageEnded - Montage end, bInterrupted: %d"), bInterrupted);

    if (!Montage || Montage != MontageToPlay)
    {
        UE_LOG(LogTemp, Warning, TEXT("UBTT_ExecuteAIAction::OnMontageEnded - wrong Montage end event"));
        return;
    }

    if (CachedOwnerComp)
    {
        UE_LOG(LogTemp, Log, TEXT("UBTT_ExecuteAIAction::OnMontageEnded - FinishLatentTask call"));
        FinishLatentTask(*CachedOwnerComp, bInterrupted ? EBTNodeResult::Failed : EBTNodeResult::Succeeded);
        CachedOwnerComp = nullptr; // 사용 후 초기화
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("UBTT_ExecuteAIAction::OnMontageEnded - CachedOwnerComp is null"));
    }
}

void UBTT_ExecuteAIAction::OnMontageBlendingOut(UAnimMontage* Montage, bool bInterrupted)
{
    UE_LOG(LogTemp, Log, TEXT("OnMontageBlendingOut - Montage Blending Out, bInterrupted: %d"), bInterrupted);

    if (!Montage || Montage != MontageToPlay)
    {
        return;
    }

    if (CachedOwnerComp)
    {
        FinishLatentTask(*CachedOwnerComp, EBTNodeResult::Succeeded);
        CachedOwnerComp = nullptr;
    }
}

void UBTT_ExecuteAIAction::FinishTask(UBehaviorTreeComponent* OwnerComp, EBTNodeResult::Type Result)
{
    UE_LOG(LogTemp, Log, TEXT("UBTT_ExecuteAIAction::FinishTask - Result: %d"), static_cast<uint8>(Result));
    if (OwnerComp)
    {
        FinishLatentTask(*OwnerComp, Result);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("UBTT_ExecuteAIAction::FinishTask - OwnerComp is null"));
    }
}