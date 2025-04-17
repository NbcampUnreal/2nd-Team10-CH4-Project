#include "AI/BTS_CheckTargetState.h"
#include "AIController.h"
#include "GameFramework/Character.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Character/Components/StateComponent.h"
#include "Kismet/GameplayStatics.h" // Need for GetNameSafe

UBTS_CheckTargetState::UBTS_CheckTargetState()
{
    NodeName = "Check Target State";

    Interval = 0.2f;
    RandomDeviation = 0.05f;
}

void UBTS_CheckTargetState::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

    UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
    if (!BlackboardComp)
    {
        return;
    }

    AAIController* AIController = OwnerComp.GetAIOwner();
    if (!AIController)
    {
        return;
    }

    AActor* TargetActor = Cast<AActor>(BlackboardComp->GetValueAsObject(TargetKey.SelectedKeyName));
    if (!TargetActor)
    {
        UE_LOG(LogTemp, Warning, TEXT("BTS_CheckTargetState: Target is not a valid Actor. Clearing state keys."));
        ClearTargetStateKeys(BlackboardComp);
        return;
    }

    ACharacter* TargetCharacter = Cast<ACharacter>(TargetActor);
    if (!TargetCharacter)
    {
        UE_LOG(LogTemp, Warning, TEXT("BTS_CheckTargetState: Target is not a valid Character. Clearing state keys."));
        ClearTargetStateKeys(BlackboardComp);
        return;
    }



    UStateComponent* TargetStateComponent = TargetCharacter->FindComponentByClass<UStateComponent>();
    if (!TargetStateComponent)
    {
        UE_LOG(LogTemp, Warning, TEXT("BTS_CheckTargetState: Target '%s' does not have a StateComponent. Clearing state keys."), *GetNameSafe(TargetCharacter));
        ClearTargetStateKeys(BlackboardComp);
        return;
    }

    ECharacterState CurrentTargetState = TargetStateComponent->GetState();

    BlackboardComp->SetValueAsEnum(TargetStateKey.SelectedKeyName, (uint8)CurrentTargetState);

    FString StateString = UEnum::GetValueAsString(CurrentTargetState);
}

void UBTS_CheckTargetState::ClearTargetStateKeys(UBlackboardComponent* BlackboardComp)
{
    if (BlackboardComp)
    {
        if (TargetStateKey.SelectedKeyName != NAME_None)
        {
            BlackboardComp->ClearValue(TargetStateKey.SelectedKeyName);
        }
    }
}