#include "AI/Character/SFEnemyCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "SFEnemyCharacter.h"

void ASFEnemyCharacter::BeginPlay()
{
    Super::BeginPlay();

    AIController = Cast<AAICharacterController>(GetController());
    if (!AIController)
    {
    }
}

void ASFEnemyCharacter::ChaseTarget()
{
    if (!AIController)
    {
        return;
    }

    UBlackboardComponent* BlackboardComp = AIController->GetBlackboardComponent();
    if (!BlackboardComp)
    {
        return;
    }

    AActor* TargetActor = Cast<AActor>(BlackboardComp->GetValueAsObject("TargetKey"));

    if (TargetActor)
    {
        AIController->MoveToLocation(TargetActor->GetActorLocation(), 100.0f, true);

        //GetWorld()->GetTimerManager().SetTimer(SeekTimerHandle, this,
        //    &ASFEnemyCharacter::SeekTarget, 0.25f, true);
    }
}
