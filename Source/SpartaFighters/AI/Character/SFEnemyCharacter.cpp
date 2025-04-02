


#include "AI/Character/SFEnemyCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "SFEnemyCharacter.h"

void ASFEnemyCharacter::BeginPlay()
{
    Super::BeginPlay();

    // AICharacterController 타입으로 캐스팅
    AIController = Cast<AAICharacterController>(GetController());
    if (!AIController)
    {
        UE_LOG(LogTemp, Warning, TEXT("AIController 캐스팅 실패!"));
    }
}

void ASFEnemyCharacter::ChaseTarget()
{
    if (!AIController)
    {
        UE_LOG(LogTemp, Warning, TEXT("AIController가 유효하지 않습니다."));
        return;
    }

    // 블랙보드에서 타겟 가져오기
    UBlackboardComponent* BlackboardComp = AIController->GetBlackboardComponent();
    if (!BlackboardComp)
    {
        UE_LOG(LogTemp, Warning, TEXT("BlackboardComponent를 찾을 수 없습니다."));
        return;
    }

    AActor* TargetActor = Cast<AActor>(BlackboardComp->GetValueAsObject("TargetKey"));

    if (TargetActor)
    {
        // 타겟 위치로 이동
        AIController->MoveToLocation(TargetActor->GetActorLocation(), 100.0f, true);

        // 주기적으로 위치 업데이트 (타겟이 움직이는 경우)
        //GetWorld()->GetTimerManager().SetTimer(SeekTimerHandle, this,
        //    &ASFEnemyCharacter::SeekTarget, 0.25f, true);
    }
}
