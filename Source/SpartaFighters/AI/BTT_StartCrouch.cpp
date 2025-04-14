


#include "BTT_StartCrouch.h"
#include "AIController.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h" // CanCrouch 확인 위해 필요

UBTT_StartCrouch::UBTT_StartCrouch()
{
    NodeName = "Start Crouch";
}

EBTNodeResult::Type UBTT_StartCrouch::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    AAIController* AIController = OwnerComp.GetAIOwner();
    if (!AIController)
    {
        UE_LOG(LogTemp, Warning, TEXT("BTT_StartCrouch: AIController is null."));
        return EBTNodeResult::Failed;
    }

    ACharacter* Character = Cast<ACharacter>(AIController->GetPawn());
    if (!Character)
    {
        UE_LOG(LogTemp, Warning, TEXT("BTT_StartCrouch: Controlled Pawn is not an ACharacter."));
        return EBTNodeResult::Failed;
    }

    // 앉을 수 있는지 확인
    if (Character->CanCrouch())
    {
        Character->Crouch(); // 앉기 시작
        // 앉기 시작 명령을 성공적으로 내렸으므로 Succeeded 반환
        // 앉기 동작이 완료될 때까지 기다리지 않음
        return EBTNodeResult::Succeeded;
    }
    else
    {
        // 이미 앉아있거나 다른 이유로 앉을 수 없는 경우
        // 만약 이미 앉아있는 상태에서 이 노드가 호출되는 것이 괜찮다면, Failed 대신 Succeeded를 반환할 수도 있음.
        // 여기서는 CanCrouch()가 false면 일단 실패 처리.
        if (Character->bIsCrouched)
        {
            UE_LOG(LogTemp, Log, TEXT("BTT_StartCrouch: Character is already crouched."));
            // 이미 앉은 상태면 성공으로 간주할 수도 있음 (디자인에 따라 결정)
            return EBTNodeResult::Succeeded;
        }
        else
        {
            UE_LOG(LogTemp, Log, TEXT("BTT_StartCrouch: Character cannot crouch right now."));
            return EBTNodeResult::Failed;
        }
    }
}
