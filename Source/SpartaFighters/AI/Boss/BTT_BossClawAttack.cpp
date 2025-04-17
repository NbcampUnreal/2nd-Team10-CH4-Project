#include "AI/Boss/BTT_BossClawAttack.h"
#include "AI/Boss/AIBossCharacter.h"
#include "AI/AICharacterController.h"

UBTT_BossClawAttack::UBTT_BossClawAttack()
{
    NodeName = TEXT("Boss Attack Claw");
}

EBTNodeResult::Type UBTT_BossClawAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    AAIBossCharacter* Boss = Cast<AAIBossCharacter>(OwnerComp.GetAIOwner()->GetPawn());
    if (Boss && Boss->HasAuthority())
    {
        Boss->AreaAttack();
        return EBTNodeResult::Succeeded;
    }
    return EBTNodeResult::Failed;
}
