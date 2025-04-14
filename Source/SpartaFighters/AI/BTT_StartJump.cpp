


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

    // 점프가 가능한지 확인 (선택적이지만 권장)
    if (Character->CanJump())
    {
        Character->Jump(); // 점프 시작
        // 점프 시작 명령을 성공적으로 내렸으므로 Succeeded 반환
        // 점프가 완료될 때까지 기다리지 않음
        return EBTNodeResult::Succeeded;
    }
    else
    {
        UE_LOG(LogTemp, Log, TEXT("BTT_StartJump: Character cannot jump right now."));
        return EBTNodeResult::Failed; // 점프할 수 없는 상태면 실패
    }
}
