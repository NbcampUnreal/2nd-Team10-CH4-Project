


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

    // 앉은 상태인지 확인 (선택적) - UnCrouch는 보통 앉아있지 않아도 문제를 일으키지 않음
    // if (!Character->bIsCrouched)
    // {
    //     UE_LOG(LogTemp, Log, TEXT("BTT_StopCrouch: Character is not crouched. Returning Succeeded anyway."));
    //     return EBTNodeResult::Succeeded; // 이미 서 있으면 성공으로 간주
    // }

    Character->UnCrouch(); // 일어서기 시작
    return EBTNodeResult::Succeeded; // 명령 전달 성공
}
