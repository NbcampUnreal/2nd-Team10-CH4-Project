


#include "AI/Boss/BTT_BossMagicAttack.h"
#include "AI/Boss/AIBossCharacter.h"
#include "AI/AICharacterController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTT_BossMagicAttack::UBTT_BossMagicAttack()
{
    NodeName = "Cast Magic Attack";
}

EBTNodeResult::Type UBTT_BossMagicAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    AAIController* AIController = OwnerComp.GetAIOwner();
    if (!AIController)
    {
        UE_LOG(LogTemp, Warning, TEXT("No AI Controller"));
        return EBTNodeResult::Failed;
    }

    AAIBossCharacter* Boss = Cast<AAIBossCharacter>(AIController->GetPawn());
    if (!Boss)
    {
        UE_LOG(LogTemp, Warning, TEXT("Pawn is not AIBossCharacter"));
        return EBTNodeResult::Failed;
    }

    Boss->StartMagicAttackSequence();

    return EBTNodeResult::Succeeded;
}
