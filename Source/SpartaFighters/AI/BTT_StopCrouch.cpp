


#include "BTT_StopCrouch.h"
#include "AIController.h"
#include "GameFramework/Character.h"

UBTT_StopCrouch::UBTT_StopCrouch()
{
    NodeName = "Stop Crouch (UnCrouch)";
}

EBTNodeResult::Type UBTT_StopCrouch::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    AAIController* AIController = OwnerComp.GetAIOwner();
    if (!AIController)
    {
        UE_LOG(LogTemp, Warning, TEXT("BTT_StopCrouch: AIController is null."));
        return EBTNodeResult::Failed;
    }

    ACharacter* Character = Cast<ACharacter>(AIController->GetPawn());
    if (!Character)
    {
        UE_LOG(LogTemp, Warning, TEXT("BTT_StopCrouch: Controlled Pawn is not an ACharacter."));
        return EBTNodeResult::Failed;
    }

     if (!Character->bIsCrouched)
     {
         UE_LOG(LogTemp, Log, TEXT("BTT_StopCrouch: Character is not crouched. Returning Succeeded anyway."));
         return EBTNodeResult::Succeeded;
     }

    Character->UnCrouch();
    return EBTNodeResult::Succeeded;
}
