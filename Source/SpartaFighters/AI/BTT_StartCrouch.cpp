


#include "BTT_StartCrouch.h"
#include "AIController.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h" // CanCrouch Ȯ�� ���� �ʿ�

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

    // ���� �� �ִ��� Ȯ��
    if (Character->CanCrouch())
    {
        Character->Crouch(); // �ɱ� ����
        // �ɱ� ���� ����� ���������� �������Ƿ� Succeeded ��ȯ
        // �ɱ� ������ �Ϸ�� ������ ��ٸ��� ����
        return EBTNodeResult::Succeeded;
    }
    else
    {
        // �̹� �ɾ��ְų� �ٸ� ������ ���� �� ���� ���
        // ���� �̹� �ɾ��ִ� ���¿��� �� ��尡 ȣ��Ǵ� ���� �����ٸ�, Failed ��� Succeeded�� ��ȯ�� ���� ����.
        // ���⼭�� CanCrouch()�� false�� �ϴ� ���� ó��.
        if (Character->bIsCrouched)
        {
            UE_LOG(LogTemp, Log, TEXT("BTT_StartCrouch: Character is already crouched."));
            // �̹� ���� ���¸� �������� ������ ���� ���� (�����ο� ���� ����)
            return EBTNodeResult::Succeeded;
        }
        else
        {
            UE_LOG(LogTemp, Log, TEXT("BTT_StartCrouch: Character cannot crouch right now."));
            return EBTNodeResult::Failed;
        }
    }
}
