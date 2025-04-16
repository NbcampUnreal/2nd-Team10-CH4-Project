


#include "AI/Boss/BTT_BossFireBreath.h"
#include "AI/Boss/AIBossCharacter.h"
#include "AI/AICharacterController.h"

UBTT_BossFireBreath::UBTT_BossFireBreath()
{
    NodeName = TEXT("Boss Attack FireBreath");
}

EBTNodeResult::Type UBTT_BossFireBreath::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    AAIBossCharacter* Boss = Cast<AAIBossCharacter>(OwnerComp.GetAIOwner()->GetPawn());
    if (Boss && Boss->HasAuthority())
    {
        Boss->StartFire();
        return EBTNodeResult::Succeeded;
    }
    return EBTNodeResult::Failed;

}
