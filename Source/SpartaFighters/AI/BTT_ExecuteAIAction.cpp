


#include "BTT_ExecuteAIAction.h"
#include "AI/AICharacterController.h"
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

    // AI ��Ʈ�ѷ� ��������
    AAIController* AIController = OwnerComp.GetAIOwner();
    if (!AIController)
    {
        UE_LOG(LogTemp, Error, TEXT("UBTT_ExecuteAIAction::ExecuteTask - AIController is Null"));
        return EBTNodeResult::Failed;
    }
    UE_LOG(LogTemp, Log, TEXT("UBTT_ExecuteAIAction::ExecuteTask - AIController Check"));

    // ���� ĳ���� ��������
    ACharacter* AICharacter = Cast<ACharacter>(AIController->GetPawn());
    if (!AICharacter)
    {
        UE_LOG(LogTemp, Error, TEXT("UBTT_ExecuteAIAction::ExecuteTask - AICharacter is Null"));
        return EBTNodeResult::Failed;
    }
    UE_LOG(LogTemp, Log, TEXT("UBTT_ExecuteAIAction::ExecuteTask - AICharacter Check"));

    // �ִϸ��̼� �ν��Ͻ� ��������
    UAnimInstance* AnimInstance = AICharacter->GetMesh()->GetAnimInstance();
    if (!AnimInstance)
    {
        UE_LOG(LogTemp, Error, TEXT("UBTT_ExecuteAIAction::ExecuteTask - AnimInstance is Null"));
        return EBTNodeResult::Failed;
    }
    UE_LOG(LogTemp, Log, TEXT("UBTT_ExecuteAIAction::ExecuteTask - AnimInstance Check"));

    if (!MontageToPlay)
    {
        UE_LOG(LogTemp, Error, TEXT("UBTT_ExecuteAIAction::ExecuteTask - MontageToPlay is Null"));
        return EBTNodeResult::Failed;
    }

    // ��Ÿ�� ���� �̺�Ʈ ���ε�
    // End Delegate ���
    MontageEndDelegate.BindUObject(this, &UBTT_ExecuteAIAction::OnMontageEnded);
    AnimInstance->Montage_SetEndDelegate(MontageEndDelegate, MontageToPlay);

    // Blending Out Delegate ���
    BlendingOutDelegate.BindUObject(this, &UBTT_ExecuteAIAction::OnMontageBlendingOut);
    AnimInstance->Montage_SetBlendingOutDelegate(BlendingOutDelegate, MontageToPlay);
    UE_LOG(LogTemp, Log, TEXT("UBTT_ExecuteAIAction::ExecuteTask - MontageEndDelegate Bind Complete"));

    GetWorld()->GetTimerManager().SetTimer(
        TimerHandle,
        [this, AICharacter, AnimInstance]() {
            if (CachedOwnerComp && !AnimInstance->Montage_IsPlaying(MontageToPlay)) {
                UE_LOG(LogTemp, Log, TEXT("UBTT_ExecuteAIAction - Montage stopped playing, finishing task"));
                FinishLatentTask(*CachedOwnerComp, EBTNodeResult::Succeeded);
                GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
            }
        },
        0.1f, // 0.1�ʸ��� Ȯ��
        true  // �ݺ�
    );

    // ��Ÿ�� ���
    float PlayRate = AnimInstance->Montage_Play(MontageToPlay);
    if (PlayRate <= 0.f)
    {
        UE_LOG(LogTemp, Error, TEXT("UBTT_ExecuteAIAction::ExecuteTask - Montage dont play"));
        return EBTNodeResult::Failed;
    }
    UE_LOG(LogTemp, Log, TEXT("UBTT_ExecuteAIAction::ExecuteTask - Montage play, PlayRate: %f"), PlayRate);

    return EBTNodeResult::InProgress;
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
        CachedOwnerComp = nullptr; // ��� �� �ʱ�ȭ
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