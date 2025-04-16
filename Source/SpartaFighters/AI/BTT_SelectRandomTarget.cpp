


#include "BTT_SelectRandomTarget.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AICharacterController.h"
#include "GameFramework/Character.h" // why use this?
#include "Kismet/GameplayStatics.h"

UBTT_SelectRandomTarget::UBTT_SelectRandomTarget()
{
	NodeName = "Random Target Select";
}

EBTNodeResult::Type UBTT_SelectRandomTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    UE_LOG(LogTemp, Warning, TEXT("Start"));
    // Get AI Controller
    AAIController* AIController = OwnerComp.GetAIOwner();
    if (!AIController)
    {
        return EBTNodeResult::Failed;
    }

    // Get AI Controlled Pawn
    APawn* SelfPawn = AIController->GetPawn();
    if (!SelfPawn)
    {
        return EBTNodeResult::Failed;
    }

    // Get All Character
    TArray<AActor*> AllCharacters;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACharacter::StaticClass(), AllCharacters);

    // TODO : Dead Character is need exeption

    // Except self
    TArray<AActor*> PossibleTargets;
    for (AActor* Actor : AllCharacters)
    {
        if (Actor != SelfPawn)
        {
            PossibleTargets.Add(Actor);
        }
    }

    // if use othermode
    //for (AActor* Actor : AllCharacters)
    //{
    //    if (Actor != SelfPawn)
    //    {
    //        if (bSelectOnlyPlayers)
    //        {
    //            APlayerController* PC = Cast<APlayerController>(Cast<ACharacter>(Actor)->GetController());
    //            if (PC)
    //            {
    //                PossibleTargets.Add(Actor);
    //            }
    //        }
    //        else if (bSelectOnlyAI)
    //        {
    //            AAIController* AIC = Cast<AAIController>(Cast<ACharacter>(Actor)->GetController());
    //            if (AIC)
    //            {
    //                PossibleTargets.Add(Actor);
    //            }
    //        }
    //        else
    //        {
    //            PossibleTargets.Add(Actor);
    //        }
    //    }

    // if None Character is fail
    if (PossibleTargets.Num() == 0)
    {
        return EBTNodeResult::Failed;
    }

    // Choice Random Character
    int32 RandomIndex = FMath::RandRange(0, PossibleTargets.Num() - 1);
    AActor* SelectedTarget = PossibleTargets[RandomIndex];
    // Set Blackboard
    UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
    if (BlackboardComp)
    {
        BlackboardComp->SetValueAsObject(TargetKey.SelectedKeyName, SelectedTarget);
        UE_LOG(LogTemp, Warning, TEXT("Selected Target: %s"), *GetNameSafe(SelectedTarget));
        return EBTNodeResult::Succeeded;
    }

    return EBTNodeResult::Failed;
}
