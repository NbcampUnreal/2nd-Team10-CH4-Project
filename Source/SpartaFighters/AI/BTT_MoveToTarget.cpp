#include "AI/BTT_MoveToTarget.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AI/AICharacterController.h"
#include "NavigationSystem.h"
#include "NavigationPath.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "TimerManager.h"

UBTT_MoveToTarget::UBTT_MoveToTarget()
{
    NodeName = TEXT("Move To Target");
    bNotifyTick = true;
    bNotifyTaskFinished = true;
    AcceptanceRadius = 150.0f;
}

EBTNodeResult::Type UBTT_MoveToTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    AIPawn = OwnerComp.GetAIOwner()->GetPawn();
    if (!AIPawn.IsValid() || !AIPawn->HasAuthority())
    {
        FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
        return EBTNodeResult::Failed;
    }

    UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
    if (!BlackboardComp)
    {
        FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
        return EBTNodeResult::Failed;
    }

    TargetActor = Cast<AActor>(BlackboardComp->GetValueAsObject(TargetKey.SelectedKeyName));
    if (!TargetActor.IsValid())
    {
        FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
        return EBTNodeResult::Failed;
    }

    bTaskFinished = false;
    // Timeout fallback
    GetWorld()->GetTimerManager().SetTimer(TimerHandle_MoveTimeout, [this, &OwnerComp]() {
        if (!bTaskFinished)
        {
            bTaskFinished = true;
            FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
            UE_LOG(LogTemp, Warning, TEXT("Move to target timed out."));
        }
    }, 10.0f, false);

    return EBTNodeResult::InProgress;
}

void UBTT_MoveToTarget::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

    if (!AIPawn.IsValid() || !TargetActor.IsValid())
    {
        bTaskFinished = true;
        FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
        return;
    }

    const FVector CurrentLocation = AIPawn->GetActorLocation();
    const FVector TargetLocation = TargetActor->GetActorLocation();

    // Check arrival
    if (FVector::Dist(CurrentLocation, TargetLocation) <= AcceptanceRadius)
    {
        GetWorld()->GetTimerManager().ClearTimer(TimerHandle_MoveTimeout);
        bTaskFinished = true;
        FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
        return;
    }

    // Find a viable movement direction on NavMesh
    FVector MoveDir = FindValidDirectionTowardsTarget(CurrentLocation, TargetLocation);
    if (!MoveDir.IsNearlyZero())
    {
        UpdateMovement(MoveDir);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("All directions blocked, cannot reach target."));
    }
}

FVector UBTT_MoveToTarget::Calculate8Direction(const FVector& CurrentLocation, const FVector& TargetLocation) const
{
    FVector Dir = (TargetLocation - CurrentLocation).GetSafeNormal2D();
    float Angle = FMath::RadiansToDegrees(FMath::Atan2(Dir.Y, Dir.X));
    float Snapped = FMath::RoundToFloat(Angle / 45.0f) * 45.0f;
    return FVector(FMath::Cos(FMath::DegreesToRadians(Snapped)),
                   FMath::Sin(FMath::DegreesToRadians(Snapped)),
                   0.0f).GetSafeNormal();
}

FVector UBTT_MoveToTarget::FindValidDirectionTowardsTarget(const FVector& CurrentLocation, const FVector& TargetLocation) const
{
    const FVector ToTarget = (TargetLocation - CurrentLocation).GetSafeNormal2D();
    float BaseAngle = FMath::RadiansToDegrees(FMath::Atan2(ToTarget.Y, ToTarget.X));

    // Check directions around the straight line
    const float Step = 45.0f;
    for (int32 i = 0; i < 8; ++i)
    {
        float CheckAngle = BaseAngle + (i <= 4 ? i * Step : (i - 8) * Step);
        FVector Dir = FVector(FMath::Cos(FMath::DegreesToRadians(CheckAngle)),
                              FMath::Sin(FMath::DegreesToRadians(CheckAngle)),
                              0.0f).GetSafeNormal();
        FVector Dest = CurrentLocation + Dir * AcceptanceRadius;
        if (IsPathValid(Dest))
        {
            return Dir;
        }
    }
    return FVector::ZeroVector;
}

bool UBTT_MoveToTarget::IsPathValid(const FVector& Destination) const
{
    UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(GetWorld());
    if (!NavSys || !AIPawn.IsValid())
    {
        return false;
    }
    UNavigationPath* Path = NavSys->FindPathToLocationSynchronously(GetWorld(), AIPawn->GetActorLocation(), Destination);
    return (Path && Path->IsValid() && Path->PathPoints.Num() > 1);
}

void UBTT_MoveToTarget::UpdateMovement(const FVector& Direction) const
{
    if (UCharacterMovementComponent* MoveComp = Cast<UCharacterMovementComponent>(AIPawn->GetMovementComponent()))
    {
        // Apply input and smooth rotation
        MoveComp->AddInputVector(Direction * 1.5f);
        FRotator TargetRot = Direction.Rotation();
        FRotator NewRot = FMath::RInterpTo(AIPawn->GetActorRotation(), TargetRot, GetWorld()->DeltaTimeSeconds, 8.0f);
        AIPawn->SetActorRotation(NewRot);
    }
}