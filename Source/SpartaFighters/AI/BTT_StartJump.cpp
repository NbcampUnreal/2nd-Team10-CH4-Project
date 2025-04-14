


#include "BTT_StartJump.h"
#include "AIController.h"
#include "GameFramework/Character.h"

UBTT_StartJump::UBTT_StartJump()
{
    NodeName = "Start Jump";
}

EBTNodeResult::Type UBTT_StartJump::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    AAIController* AIController = OwnerComp.GetAIOwner();
    if (!AIController)
    {
        UE_LOG(LogTemp, Warning, TEXT("BTT_StartJump: AIController is null."));
        return EBTNodeResult::Failed;
    }

    ACharacter* Character = Cast<ACharacter>(AIController->GetPawn());
    if (!Character)
    {
        UE_LOG(LogTemp, Warning, TEXT("BTT_StartJump: Controlled Pawn is not an ACharacter."));
        return EBTNodeResult::Failed;
    }

    if (Character->CanJump())
    {
        Character->Jump();
        return EBTNodeResult::Succeeded;
    }
    else
    {
        UE_LOG(LogTemp, Log, TEXT("BTT_StartJump: Character cannot jump right now."));
        return EBTNodeResult::Failed;
    }
}
