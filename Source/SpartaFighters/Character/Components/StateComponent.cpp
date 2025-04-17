#include "Character/Components/StateComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Net/UnrealNetwork.h"

UStateComponent::UStateComponent()
{
	CurrentState = ECharacterState::Idle;
	CurrentSpecialState = ECharacterSpecialState::None;

	SetIsReplicatedByDefault(true);
	bIsInAction = false;
}

void UStateComponent::BeginPlay()
{
	Super::BeginPlay();

	//if (GetOwnerRole() == ROLE_Authority)
	//{
	//	if (ACharacter* Character = Cast<ACharacter>(GetOwner()))
	//	{
	//		UpdateState(Character);
	//	}
	//}

}

void UStateComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UStateComponent, CurrentState);
	DOREPLIFETIME(UStateComponent, bIsInAction);
}


void UStateComponent::SetState(ECharacterState NewState)
{
	if (CurrentState != NewState)
	{
		CurrentState = NewState;
	}
}


void UStateComponent::UpdateState(ACharacter* Character)
{
	if (!Character) return;

	auto Authority = Character->HasAuthority();
	ensureAlways(Authority);

	UCharacterMovementComponent* Movement = Character->GetCharacterMovement();
	if (!Movement) return;

	if (Movement->IsFalling())
	{
		CurrentState = ECharacterState::InAir;
	}
	else if (Character->bIsCrouched)
	{
		CurrentState = ECharacterState::Crouching;
	}
	else if (Movement->Velocity.SizeSquared() > 10.f)
	{
		CurrentState = ECharacterState::Moving;
	}
	else
	{
		CurrentState = ECharacterState::Idle;
	}
}

void UStateComponent::OnRep_CurrentState()
{
	UE_LOG(LogTemp, Log, TEXT("[Client] MovementState changed to: %d"), (uint8)CurrentState);
}

void UStateComponent::OnRep_SpecialState()
{
	UE_LOG(LogTemp, Log, TEXT("[Client] SpecialState changed to: %d"), (uint8)CurrentSpecialState);
}


void UStateComponent::SetIsInAction(bool bIn)
{
	bIsInAction = bIn;
}

bool UStateComponent::IsInAction() const
{
	return bIsInAction;
}


bool UStateComponent::IsInState(ECharacterState CheckState) const
{
	return CurrentState == CheckState;
}

void UStateComponent::SetSpecialState(ECharacterSpecialState NewState)
{
	CurrentSpecialState = NewState;
}

void UStateComponent::ResetSpecialState()
{
	CurrentSpecialState = ECharacterSpecialState::None;
}

