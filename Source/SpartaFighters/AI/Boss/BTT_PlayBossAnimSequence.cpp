#include "AI/Boss/BTT_PlayBossAnimSequence.h"
#include "AI/Boss/AIBossCharacter.h"
#include "AIController.h"

UBTT_PlayBossAnimSequence::UBTT_PlayBossAnimSequence()
{
    NodeName = TEXT("Play Boss Anim Sequence");
    bNotifyTick = true;
}

EBTNodeResult::Type UBTT_PlayBossAnimSequence::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    AAIController* AIController = OwnerComp.GetAIOwner();
    if (!AIController)
    {
        UE_LOG(LogTemp, Error, TEXT("[BTT_PlayBossAnimSequence] AIController null"));
        return EBTNodeResult::Failed;
    }

    BossCharacter = Cast<AAIBossCharacter>(AIController->GetPawn());
    if (!BossCharacter.IsValid())
    {
        UE_LOG(LogTemp, Error, TEXT("[BTT_PlayBossAnimSequence] BossCharacter cast fail"));
        return EBTNodeResult::Failed;
    }

    if (!AttackSequence)
    {
        UE_LOG(LogTemp, Error, TEXT("[BTT_PlayBossAnimSequence] AttackSequence null"));
        return EBTNodeResult::Failed;
    }

    if (BossCharacter->HasAuthority())
    {
        BossCharacter->Server_PlayAttack(AttackSequence);
        bIsPlaying = true;
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("[BTT_PlayBossAnimSequence] client start"));
    }

    return EBTNodeResult::InProgress;
}

void UBTT_PlayBossAnimSequence::TickTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, float DeltaSeconds)
{
    if (BossCharacter.IsValid() && !BossCharacter->bIsPlaying)
    {
        FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
    }
}
