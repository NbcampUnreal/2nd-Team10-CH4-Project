#include "Character/Components/MovementInputComponent.h"
#include "Character/Components/StatusContainerComponent.h"
#include "EnhancedInputComponent.h"
#include "Character/SFCharacter.h"
#include "Framework/SFPlayerController.h"

UMovementInputComponent::UMovementInputComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	JumpCount = 0;

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

	if (IsValid(PlayerController->MoveAction))
	{
		Input->BindAction(PlayerController->MoveAction, ETriggerEvent::Triggered, this, &UMovementInputComponent::Move);
	}

	if (IsValid(PlayerController->JumpAction))
	{
		Input->BindAction(PlayerController->JumpAction, ETriggerEvent::Started, this, &UMovementInputComponent::StartJump);
		Input->BindAction(PlayerController->JumpAction, ETriggerEvent::Completed, this, &UMovementInputComponent::StopJump);
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

	// Make Direction Vector
	FVector InputDirection = FVector(InputVector.X, InputVector.Y, 0.f);

	if (!InputDirection.IsNearlyZero())
	{
		// Get Only Direction
		InputDirection.Normalize();

		// Direction -> Rotation
		FRotator TargetRotation = InputDirection.Rotation();

		// I needs only Yaw
		TargetRotation.Pitch = 0.f;
		TargetRotation.Roll = 0.f;

		OwnerCharacter->SetActorRotation(TargetRotation);
	}
}

void UMovementInputComponent::StartJump(const FInputActionValue& Value)
{
	// 2 Jump !!
	if (JumpCount < OwnerCharacter->JumpMaxCount)
	{
		OwnerCharacter->Jump();
		JumpCount++;
	}
}

void UMovementInputComponent::StopJump(const FInputActionValue& Value)
{
	OwnerCharacter->StopJumping();
}

void UMovementInputComponent::ResetJumpCount()
{
	JumpCount = 0; // 이게 없으면 1번 점프 후 끝남
}

void UMovementInputComponent::StartRoll(const FInputActionValue& Value)
{
	//if (bIsRolling || !CanRoll()) return;

	//bIsRolling = true;
	//PlayAnimMontage(RollMontage);

	//FVector RollDir = GetLastMovementInputVector();
	//if (!RollDir.IsNearlyZero())
	//{
	//	RollDir.Normalize();
	//	LaunchCharacter(RollDir * RollPower, true, true);
	//}

	//// 구르기 끝났을 때 상태 복구
	//GetWorld()->GetTimerManager().SetTimer(RollTimerHandle, this, &AMyCharacter::EndRoll, RollDuration, false);
}

void UMovementInputComponent::EndRoll(const FInputActionValue& Value)
{
	/*bIsRolling = false;*/
}
