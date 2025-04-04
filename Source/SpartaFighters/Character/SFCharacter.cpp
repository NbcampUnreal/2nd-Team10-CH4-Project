#include "Character/SFCharacter.h"

#include "EnhancedInputComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Framework/SFPlayerController.h"
#include "Components/MovementInputComponent.h"
#include "Components/StatusContainerComponent.h"

#include "DataTable/SkillDataRow.h"
#include "Animation/AnimInstance.h"

ASFCharacter::ASFCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

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

void ASFCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASFCharacter::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.X);
		AddMovementInput(RightDirection, MovementVector.Y);
	}
}

void ASFCharacter::StartJump(const FInputActionValue& Value)
{
	ACharacter::Jump();

}

void ASFCharacter::StopJump(const FInputActionValue& Value)
{
	ACharacter::StopJumping();

}

void ASFCharacter::StartRoll(const FInputActionValue& Value)
{
	bIsRoll = true;

}

void ASFCharacter::StopRoll(const FInputActionValue& Value)
{
	
}

void ASFCharacter::CrouchPressed(const FInputActionValue& Value)
{
	bIsCrouch = true;

}

void ASFCharacter::CrouchReleased(const FInputActionValue& Value)
{
	bIsCrouch = false;

}

void ASFCharacter::AttackPressed(const FInputActionValue& Value)
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
}

void ASFCharacter::AttackReleased(const FInputActionValue& Value)
{

}

void ASFCharacter::SkillAttackPressed(const FInputActionValue& Value)
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

void ASFCharacter::SkillAttackReleased(const FInputActionValue& Value)
{

}

void ASFCharacter::GuardPressed(const FInputActionValue& Value)
{
	bIsGuard = true;

}

void ASFCharacter::GuardReleased(const FInputActionValue& Value)
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

			EnhancedInput->BindAction(SFPlayerController->RollAction, ETriggerEvent::Started, this, &ASFCharacter::SkillAttackPressed);
			EnhancedInput->BindAction(SFPlayerController->RollAction, ETriggerEvent::Completed, this, &ASFCharacter::SkillAttackReleased);

			EnhancedInput->BindAction(SFPlayerController->CrouchAction, ETriggerEvent::Started, this, &ASFCharacter::CrouchPressed);
			EnhancedInput->BindAction(SFPlayerController->CrouchAction, ETriggerEvent::Completed, this, &ASFCharacter::CrouchReleased);

			// Ability
			EnhancedInput->BindAction(SFPlayerController->AttackAction, ETriggerEvent::Started, this, &ASFCharacter::AttackPressed);
			EnhancedInput->BindAction(SFPlayerController->AttackAction, ETriggerEvent::Completed, this, &ASFCharacter::AttackReleased);

			EnhancedInput->BindAction(SFPlayerController->SkillAttackAction, ETriggerEvent::Started, this, &ASFCharacter::SkillAttackPressed);
			EnhancedInput->BindAction(SFPlayerController->SkillAttackAction, ETriggerEvent::Completed, this, &ASFCharacter::SkillAttackReleased);

			EnhancedInput->BindAction(SFPlayerController->GuardAction, ETriggerEvent::Started, this, &ASFCharacter::GuardPressed);
			EnhancedInput->BindAction(SFPlayerController->GuardAction, ETriggerEvent::Completed, this, &ASFCharacter::GuardReleased);

			// Other SetupInput() gogogo
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
