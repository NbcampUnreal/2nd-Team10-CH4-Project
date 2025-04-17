#include "BTT_StopJump.h"
#include "AIController.h"
#include "GameFramework/Character.h"

UBTT_StopJump::UBTT_StopJump()
{
    NodeName = "Stop Jump";
}

EBTNodeResult::Type UBTT_StopJump::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    AAIController* AIController = OwnerComp.GetAIOwner();
    if (!AIController)
    {
        UE_LOG(LogTemp, Warning, TEXT("BTT_StopJump: AIController is null."));
        return EBTNodeResult::Failed;
    }

    ACharacter* Character = Cast<ACharacter>(AIController->GetPawn());
    if (!Character)
    {
        UE_LOG(LogTemp, Warning, TEXT("BTT_StopJump: Controlled Pawn is not an ACharacter."));
        return EBTNodeResult::Failed;
    }

    Character->StopJumping();
    return EBTNodeResult::Succeeded;
}
