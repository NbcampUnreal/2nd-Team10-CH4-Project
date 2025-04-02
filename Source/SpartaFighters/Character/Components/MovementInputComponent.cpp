#include "Character/Components/MovementInputComponent.h"
#include "EnhancedInputComponent.h"
#include "Framework/SFPlayerController.h"
#include "Character/SFCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

UMovementInputComponent::UMovementInputComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void UMovementInputComponent::BeginPlay()
{
	Super::BeginPlay();
	
}

void UMovementInputComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UMovementInputComponent::SetupInput(UEnhancedInputComponent* Input, ASFPlayerController* PlayerController, ASFCharacter* SFCharacter)
{
	OwnerCharacter = SFCharacter;

	if (OwnerCharacter && OwnerCharacter->GetCharacterMovement())
	{
		OwnerCharacter->GetCharacterMovement()->JumpZVelocity = 1000.f;
		OwnerCharacter->GetCharacterMovement()->AirControl = 1.0f;
	}

	if (IsValid(PlayerController->MoveAction))
	{
		Input->BindAction(PlayerController->MoveAction, ETriggerEvent::Triggered, this, &UMovementInputComponent::Move);
	}

	if (IsValid(PlayerController->JumpAction))
	{
		Input->BindAction(PlayerController->JumpAction, ETriggerEvent::Started, this, &UMovementInputComponent::JumpStart);
		Input->BindAction(PlayerController->JumpAction, ETriggerEvent::Completed, this, &UMovementInputComponent::JumpStop);
	}
}

void UMovementInputComponent::Move(const FInputActionValue& Value)
{
	FVector2D InputVector = Value.Get<FVector2D>();

	FVector FrontBack = OwnerCharacter->GetActorForwardVector();
	FVector LeftRight = OwnerCharacter->GetActorRightVector();

	// In Unreal X direction is FrontBack.
	OwnerCharacter->AddMovementInput(FrontBack, InputVector.X);
	OwnerCharacter->AddMovementInput(LeftRight, InputVector.Y);
}

void UMovementInputComponent::JumpStart(const FInputActionValue& Value)
{
	OwnerCharacter->Jump();
}

void UMovementInputComponent::JumpStop(const FInputActionValue& Value)
{
	OwnerCharacter->StopJumping();
}
