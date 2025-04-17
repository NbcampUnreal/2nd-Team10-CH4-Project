#include "AI/BTT_MoveToTarget.h"
#include "AI/AICharacterController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Actor.h"
#include "GameFramework/CharacterMovementComponent.h"

UBTT_MoveToTarget::UBTT_MoveToTarget()
{
    NodeName = "Move To Target";
    bNotifyTick = true;
    bNotifyTaskFinished = true;
    AcceptanceRadius = 150.0f;
}

EBTNodeResult::Type UBTT_MoveToTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    AIPawn = OwnerComp.GetAIOwner()->GetPawn();

    if (!AIPawn || !AIPawn->HasAuthority())
    {
        FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
        return EBTNodeResult::Failed;
    }

    UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();

    if (!BlackboardComp)
    {
        return EBTNodeResult::Failed;
    }

    TargetCharacter = Cast<AActor>(BlackboardComp->GetValueAsObject(TargetKey.SelectedKeyName));

    if (!TargetCharacter)
    {
        return EBTNodeResult::Failed;
    }

    // if there is a problem with this, force quit
    GetWorld()->GetTimerManager().SetTimer(TimerHandle_MoveTimeout, [&]()
        {
            if (!bTaskFinished)
            {
                FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
                UE_LOG(LogTemp, Warning, TEXT("Move timed out!"));
            }
        }, 10.0f, false);

    return EBTNodeResult::InProgress;
}

void UBTT_MoveToTarget::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

    if (!AIPawn || !TargetCharacter)
    {
        FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
        return;
    }

    const FVector TargetLocation = TargetCharacter->GetActorLocation();
    const FVector CurrentLocation = AIPawn->GetActorLocation();

    if (FVector::Dist(CurrentLocation, TargetLocation) <= AcceptanceRadius)
    {
        GetWorld()->GetTimerManager().ClearTimer(TimerHandle_MoveTimeout);
        FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
        return;
    }

    const FVector MovementDirection = Calculate8Direction(CurrentLocation, TargetLocation);
    UpdateMovement(MovementDirection);
}

FVector UBTT_MoveToTarget::Calculate8Direction(const FVector& CurrentLocation, const FVector& TargetLocation) const
{
    const FVector Direction = (TargetLocation - CurrentLocation).GetSafeNormal2D();
    const float Angle = FMath::RadiansToDegrees(FMath::Atan2(Direction.Y, Direction.X));
    const float SnappedAngle = FMath::RoundToFloat(Angle / 45.0f) * 45.0f;

    return FVector(
        FMath::Cos(FMath::DegreesToRadians(SnappedAngle)),
        FMath::Sin(FMath::DegreesToRadians(SnappedAngle)),
        0
    ).GetSafeNormal();
}

void UBTT_MoveToTarget::UpdateMovement(const FVector& Direction) const
{
    if (UCharacterMovementComponent* MovementComp = Cast<UCharacterMovementComponent>(AIPawn->GetMovementComponent()))
    {
        MovementComp->AddInputVector(Direction * 1.5f);

        const FRotator TargetRotation = Direction.Rotation();
        const FRotator NewRotation = FMath::RInterpTo(AIPawn->GetActorRotation(),TargetRotation, GetWorld()->GetDeltaSeconds(), 8.0f);

        AIPawn->SetActorRotation(NewRotation);
    }
}