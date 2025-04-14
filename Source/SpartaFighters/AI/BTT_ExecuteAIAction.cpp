


#include "BTT_ExecuteAIAction.h"
#include "AI/AICharacterController.h"
#include "Character/SFCharacter.h"
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
    UE_LOG(LogTemp, Log, TEXT("UBTT_ExecuteAIAction::ExecuteTask - Start"));

    CachedOwnerComp = &OwnerComp;

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

    ACharacter* AICharacter = Cast<ACharacter>(AIController->GetPawn());
    if (!AICharacter)
    {
        UE_LOG(LogTemp, Error, TEXT("UBTT_ExecuteAIAction::ExecuteTask - AICharacter is Null"));
        return EBTNodeResult::Failed;
    }

    UAnimInstance* AnimInstance = AICharacter->GetMesh()->GetAnimInstance();
    if (!AnimInstance)
    {
        UE_LOG(LogTemp, Error, TEXT("UBTT_ExecuteAIAction::ExecuteTask - AnimInstance is Null"));
        return EBTNodeResult::Failed;
    }

    ASFCharacter* SFCharacter = Cast<ASFCharacter>(AICharacter);
    if (!SFCharacter)
    {
        UE_LOG(LogTemp, Error, TEXT("UBTT_ExecuteAIAction::ExecuteTask - Cast to ASFCharacter failed"));
        return EBTNodeResult::Failed;
    }

    uint8 AIAction = BlackboardComp->GetValueAsEnum(AIActionKey.SelectedKeyName);

    EAIActionType AIActionType = static_cast<EAIActionType>(AIAction);

    //switch (AIActionType)
    //{
    //case EAIActionType::Attack:
    //    SFCharacter->PerformAttack(0);
    //    break;

    //case EAIActionType::Guard:
    //    SFCharacter->PerformAttack(0);
    //    break;

    //case EAIActionType::Evade:
    //    SFCharacter->PerformAttack(0);
    //    break;

    //case EAIActionType::Retreat:
    //    SFCharacter->PerformAttack(0);
    //    break;
    //    
    //case EAIActionType::Chase:
    //    return EBTNodeResult::Failed;

    //default:
    //    UE_LOG(LogTemp, Warning, TEXT("Invalid AIAction Enum."));
    //    return EBTNodeResult::Failed;
    //}

    return EBTNodeResult::Succeeded;

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