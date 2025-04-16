#include "BTT_PlayBossAnim.h"
#include "AIController.h"

UBTT_PlayBossAnim::UBTT_PlayBossAnim()
{
    NodeName = TEXT("Play Boss Animation");
    bNotifyTick = true;
}

EBTNodeResult::Type UBTT_PlayBossAnim::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    AAIController* AIController = OwnerComp.GetAIOwner();
    if (!AIController) return EBTNodeResult::Failed;

    BossCharacter = Cast<AAIBossCharacter>(AIController->GetPawn());
    if (!BossCharacter.IsValid() || !MontageToPlay) return EBTNodeResult::Failed;

    if (BossCharacter->HasAuthority())
    {
        BossCharacter->PlayAnimMontage(MontageToPlay, 1.0f, NAME_None);
    
        UAnimInstance* AnimInstance = BossCharacter->GetMesh()->GetAnimInstance();
        if (AnimInstance)
        {
            MontageEndedDelegate.BindUObject(this, &UBTT_PlayBossAnim::OnMontageEnded);
            AnimInstance->Montage_SetEndDelegate(MontageEndedDelegate, MontageToPlay);
        }

        BossCharacter->Multicast_PlayBossAnim(MontageToPlay);
        bIsMontagePlaying = true;
    }

    return EBTNodeResult::InProgress;
}

void UBTT_PlayBossAnim::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    if (!bIsMontagePlaying)
    {
        FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
    }
}

void UBTT_PlayBossAnim::OnMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
    bIsMontagePlaying = false;
}