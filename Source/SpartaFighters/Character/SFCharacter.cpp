#include "Character/SFCharacter.h"

#include "EnhancedInputComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "TimerManager.h"

#include "Framework/SFPlayerController.h"
#include "Components/MovementInputComponent.h"
#include "Components/StatusContainerComponent.h"

#include "DataTable/SkillDataRow.h"
#include "Animation/AnimInstance.h"

ASFCharacter::ASFCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	// Init Character Movement
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);

	// Create a Spring Arm
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 800.0f;
	SpringArm->bUsePawnControlRotation = true;

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	// Add Components
	//MovementInputComponent = CreateDefaultSubobject<UMovementInputComponent>(TEXT("MoveInputComponent"));

	StatusContainerComponent = CreateDefaultSubobject<UStatusContainerComponent>(TEXT("StatusContainerComponent"));
	StatusContainerComponent->InitializeMovementProperties(this);


}

UStatusContainerComponent* ASFCharacter::GetStatusContainerComponent() const
{
	return StatusContainerComponent;
}

void ASFCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	//InitializeCharacterProperties();
}

void ASFCharacter::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// Find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// Get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// Get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// Add movement 
		AddMovementInput(ForwardDirection, MovementVector.X);
		AddMovementInput(RightDirection, MovementVector.Y);
	}
}

void ASFCharacter::StartJump()
{
	ACharacter::Jump();
	// TO DO : Low Jump Function
	//GetWorld()->GetTimerManager().SetTimer(JumpHoldTimer, this, &ASFCharacter::StopJump, MaxJumpHoldTime, false);
}

void ASFCharacter::StopJump()
{
	ACharacter::StopJumping();

	// TO DO : Low Jump Function
	//GetWorldTimerManager().ClearTimer(JumpHoldTimer);
}

void ASFCharacter::RollPressed(const FInputActionValue& Value)
{
	bIsRoll = true;

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && SkillDataTable)
	{
		static const FString ContextString(TEXT("SkillDataLookup"));

		FName TargetRowName = FName(TEXT("RollSkill"));
		FSkillDataRow* SkillData = SkillDataTable->FindRow<FSkillDataRow>(TargetRowName, ContextString);

		if (SkillData && SkillData->SkillMontage)
		{
			AnimInstance->Montage_Play(SkillData->SkillMontage);
		}
	}
}

void ASFCharacter::RollReleased(const FInputActionValue& Value)
{
	bIsRoll = false;
}

//void ASFCharacter::StartRoll(const FInputActionValue& Value)
//{
//	bIsRoll = true;
//
//}
//
//void ASFCharacter::StopRoll(const FInputActionValue& Value)
//{
//	
//}

void ASFCharacter::CrouchPressed()
{
	bIsCrouch = true;

}

void ASFCharacter::CrouchReleased()
{
	bIsCrouch = false;

}

void ASFCharacter::AttackPressed()
{
	//PerformAttack(0); // Example of performing the first attack in the array

	bIsAttack = true;

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && SkillDataTable)
	{
		FName TargetRowName;

		// TO DO : Sperate State 
		if (GetCharacterMovement()->IsFalling())
		{
			TargetRowName = FName(TEXT("JumpBaseAttack"));
		}
		else if (bIsCrouch)
		{
			TargetRowName = FName(TEXT("CrouchBaseAttack"));
		}
		else if (UKismetMathLibrary::VSizeXY(GetCharacterMovement()->Velocity) >= 3
			&& GetCharacterMovement()->GetCurrentAcceleration() != FVector().Zero())
		{
			TargetRowName = FName(TEXT("MoveBaseAttack"));
		}
		else
		{
			TargetRowName = FName(TEXT("BaseAttack_1"));
		}

		static const FString ContextString(TEXT("SkillDataLookup"));
		CurrentSkillDataBuffer = SkillDataTable->FindRow<FSkillDataRow>(TargetRowName, ContextString);

		if (CurrentSkillDataBuffer && CurrentSkillDataBuffer->SkillMontage)
		{
			AnimInstance->Montage_Play(CurrentSkillDataBuffer->SkillMontage);
		}
	}
}

void ASFCharacter::AttackReleased()
{

}

void ASFCharacter::SkillAttackPressed()
{
	bIsAttack = true;

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && SkillDataTable)
	{
		static const FString ContextString(TEXT("SkillDataLookup"));

		FName TargetRowName = FName(TEXT("IdleSkill"));
		FSkillDataRow* SkillData = SkillDataTable->FindRow<FSkillDataRow>(TargetRowName, ContextString);

		if (SkillData && SkillData->SkillMontage)
		{
			AnimInstance->Montage_Play(SkillData->SkillMontage);
		}
	}

}

void ASFCharacter::SkillAttackReleased()
{

}

void ASFCharacter::GuardPressed()
{
	bIsGuard = true;

}

void ASFCharacter::GuardReleased()
{
	bIsGuard = false;

}


float ASFCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	return 0.0f;
}



// TO DO : Seperate Components
void ASFCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (ASFPlayerController* SFPlayerController = Cast<ASFPlayerController>(GetController()))
	{
		if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
		{
			// Movement
			//MovementInputComponent->SetupInput(EnhancedInput, SFPlayerController, this);
			EnhancedInput->BindAction(SFPlayerController->MoveAction, ETriggerEvent::Triggered, this, &ASFCharacter::Move);

			EnhancedInput->BindAction(SFPlayerController->JumpAction, ETriggerEvent::Started, this, &ASFCharacter::StartJump);
			EnhancedInput->BindAction(SFPlayerController->JumpAction, ETriggerEvent::Completed, this, &ASFCharacter::StopJump);

			EnhancedInput->BindAction(SFPlayerController->RollAction, ETriggerEvent::Started, this, &ASFCharacter::RollPressed);
			EnhancedInput->BindAction(SFPlayerController->RollAction, ETriggerEvent::Completed, this, &ASFCharacter::RollReleased);

			EnhancedInput->BindAction(SFPlayerController->CrouchAction, ETriggerEvent::Started, this, &ASFCharacter::CrouchPressed);
			EnhancedInput->BindAction(SFPlayerController->CrouchAction, ETriggerEvent::Completed, this, &ASFCharacter::CrouchReleased);

			// Ability
			EnhancedInput->BindAction(SFPlayerController->AttackAction, ETriggerEvent::Started, this, &ASFCharacter::AttackPressed);
			EnhancedInput->BindAction(SFPlayerController->AttackAction, ETriggerEvent::Completed, this, &ASFCharacter::AttackReleased);

			EnhancedInput->BindAction(SFPlayerController->SkillAttackAction, ETriggerEvent::Started, this, &ASFCharacter::SkillAttackPressed);
			EnhancedInput->BindAction(SFPlayerController->SkillAttackAction, ETriggerEvent::Completed, this, &ASFCharacter::SkillAttackReleased);

			EnhancedInput->BindAction(SFPlayerController->GuardAction, ETriggerEvent::Started, this, &ASFCharacter::GuardPressed);
			EnhancedInput->BindAction(SFPlayerController->GuardAction, ETriggerEvent::Completed, this, &ASFCharacter::GuardReleased);

			// TO DO : Other Setup ex) Pause, ......
		}
	}

}

//void ASFCharacter::InitializeCharacterProperties()
//{
//	if (StatusContainerComponent)
//	{
//		StatusContainerComponent->InitializeMovementProperties(this);
//	}
//
//	// later add Example
//	// ex) CombatComponent->InitializeCombatProperties();
//}
//
//void ASFCharacter::Landed(const FHitResult& Hit)
//{
//	//Super::Landed(Hit);
//
//	// Needs Reset
//	if (MovementInputComponent)
//	{
//		MovementInputComponent->ResetJumpCount();
//	}
//}

void ASFCharacter::PerformAttack(int32 AttackIndex)
{
	bIsAttack = true;

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance)
	{
		TArray<FSkillDataRow*> AllSkils;
		static const FString ContextString(TEXT("SkillDataText"));
		SkillDataTable->GetAllRows(ContextString, AllSkils);

		if (AllSkils[0])
		{
			if (AllSkils[0]->SkillMontage)
			{
				AnimInstance->Montage_Play(AllSkils[0]->SkillMontage);
			}
		}
	}

	if (AttackHandlers.IsValidIndex(AttackIndex))
	{
		if (IHandleAttack* Handler = Cast<IHandleAttack>(AttackHandlers[AttackIndex]))
		{
			Handler->PerformAttack();
		}
	}
}

void ASFCharacter::AddAttackHandler(UObject* AttackHandler)
{
	AttackHandlers.Add(AttackHandler);
}


void ASFCharacter::AttackTrace()
{
	FVector SocketLocation = GetMesh()->GetSocketLocation(CurrentSkillDataBuffer->SocketLocation);

	float TraceLength = CurrentSkillDataBuffer->TraceLength;
	float TraceLadius = CurrentSkillDataBuffer->TraceRadius;

	FVector StartLocation = SocketLocation - GetActorForwardVector() * TraceLength / 2;
	FVector EndLocation = SocketLocation + GetActorForwardVector() * TraceLength / 2;

	FHitResult HitResult;

	// Capsule Trace
	bool bHit = GetWorld()->SweepSingleByChannel(
		HitResult,
		StartLocation,
		EndLocation,
		FQuat::Identity,
		ECollisionChannel::ECC_Pawn,
		FCollisionShape::MakeCapsule(TraceLadius, TraceLadius)
	);

	DrawDebugCapsule(
		GetWorld(),
		(StartLocation + EndLocation) / 2,
		TraceLength / 2,
		TraceLadius,
		FRotationMatrix::MakeFromZ(EndLocation - StartLocation).ToQuat(),
		bHit ? FColor::Red : FColor::Green,
		false, 1.0f
	);

	if (bHit && HitResult.GetActor())
	{
		// 데미지 적용
		UGameplayStatics::ApplyPointDamage(
			HitResult.GetActor(),
			10.f,
			EndLocation - StartLocation,
			HitResult,
			GetController(),
			this,
			UDamageType::StaticClass()
		);
	}
}
