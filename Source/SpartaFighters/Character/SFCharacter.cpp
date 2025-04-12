#include "Character/SFCharacter.h"

#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"

#include "Framework/SFPlayerController.h"
#include "Framework/SFBattleGameMode.h"
#include "Components/StatusContainerComponent.h"
#include "Components/StateComponent.h"
#include "Components/SkillComponent.h"

#include "DataTable/SkillDataRow.h"

#include "Net/UnrealNetwork.h"

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
	SpringArm->TargetArmLength = 600.0f;
	SpringArm->bUsePawnControlRotation = true;
	SpringArm->SocketOffset.Z = 200.0f;
	SpringArm->bDoCollisionTest = false;

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;
	FollowCamera->AddLocalRotation(FRotator(-10.0f, 0.0f, 0.0f));

	// Add Components
	//MovementInputComponent = CreateDefaultSubobject<UMovementInputComponent>(TEXT("MoveInputComponent"));

	StatusContainerComponent = CreateDefaultSubobject<UStatusContainerComponent>(TEXT("StatusContainerComponent"));	
	StateComponent = CreateDefaultSubobject<UStateComponent>(TEXT("StateComponent"));
	SkillComponent = CreateDefaultSubobject<USkillComponent>(TEXT("SkillComponent"));
	
	bReplicates = true;
	GetCharacterMovement()->SetIsReplicated(true);
}

void ASFCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

}

//UStatusContainerComponent* ASFCharacter::GetStatusContainerComponent() const
//{
//	return StatusContainerComponent;
//}

void ASFCharacter::BeginPlay()
{
	Super::BeginPlay();

	StatusContainerComponent->InitStatusComponent(this);
	//StateComponent->Init();
	SkillComponent->Initialize(SkillDataTable, StateComponent, this);
	
}

void ASFCharacter::InitCharacter()
{
	// TO DO : Accessary or Equipped Item Initialize

}

void ASFCharacter::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		if (!StateComponent->CanMove()) return;

		// Find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// Get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// Get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(ForwardDirection, MovementVector.X);
		AddMovementInput(RightDirection, MovementVector.Y);
	}
}

void ASFCharacter::StartJump()
{
	if (!StateComponent->CanJump()) return;

	ACharacter::Jump();
}

void ASFCharacter::StopJump()
{
	ACharacter::StopJumping();

}

void ASFCharacter::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);
}

void ASFCharacter::RollPressed()
{
	// TO DO : Move To SKill Component
	if (StateComponent->IsInAction()) return;
	SkillComponent->HandleInputRoll();
}

void ASFCharacter::RollReleased()
{

}


void ASFCharacter::CrouchPressed()
{
	if (!StateComponent->CanCrouch()) return;

	Crouch();

}

void ASFCharacter::CrouchReleased()
{
	UnCrouch();
}

void ASFCharacter::AttackPressed()
{
	if (!SkillComponent || !StateComponent) return;
	SkillComponent->HandleInputBasicAttack();
}

void ASFCharacter::AttackReleased()
{
	// For Command

}

void ASFCharacter::SkillAttackPressed()
{
	if (!SkillComponent || !StateComponent) return;
	SkillComponent->HandleInputSkillAttack();
}

void ASFCharacter::SkillAttackReleased()
{

}

void ASFCharacter::GuardPressed()
{


}

void ASFCharacter::GuardReleased()
{

}

void ASFCharacter::InteractionPressed()
{

}

void ASFCharacter::SettingPressed()
{

}

void ASFCharacter::AttackTrace()
{
	if (SkillComponent)
	{
		SkillComponent->PerformAttackTrace();
	}
}


// TO DO : Damage Component
float ASFCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	auto hasAuthority = HasAuthority();
	ensureAlways(hasAuthority);

	if (StatusContainerComponent)
	{
		StatusContainerComponent->ModifyStatus(EStatusType::CurHP, DamageAmount);
	}

	Multicast_TakeDamageOnServer(DamageAmount, DamageEvent, DamageCauser);

	return 0.0f;
}

void ASFCharacter::Multicast_TakeDamageOnServer_Implementation(const float Damage, const FDamageEvent& DamageEvent, AActor* DamageCauser)
{

	// TO DO : 
	if (OnDamageMontage)
	{
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (AnimInstance)
		{
			AnimInstance->Montage_Play(OnDamageMontage);
		}
	}

	//if()
	// TO DO : 

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
			EnhancedInput->BindAction(SFPlayerController->InteractionAction, ETriggerEvent::Started, this, &ASFCharacter::InteractionPressed);
			EnhancedInput->BindAction(SFPlayerController->InteractionAction, ETriggerEvent::Started, this, &ASFCharacter::InteractionPressed);
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

//void ASFCharacter::PerformAttack(int32 AttackIndex)
//{
//	//bIsAttack = true;
//
//	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
//	if (AnimInstance)
//	{
//		TArray<FSkillDataRow*> AllSkils;
//		static const FString ContextString(TEXT("SkillDataText"));
//		SkillDataTable->GetAllRows(ContextString, AllSkils);
//
//		if (AllSkils[0])
//		{
//			if (AllSkils[0]->SkillMontage)
//			{
//				AnimInstance->Montage_Play(AllSkils[0]->SkillMontage);
//			}
//		}
//	}
//
//	if (AttackHandlers.IsValidIndex(AttackIndex))
//	{
//		if (IHandleAttack* Handler = Cast<IHandleAttack>(AttackHandlers[AttackIndex]))
//		{
//			Handler->PerformAttack();
//		}
//	}
//}
//
//void ASFCharacter::AddAttackHandler(UObject* AttackHandler)
//{
//	AttackHandlers.Add(AttackHandler);
//}

void ASFCharacter::Die()
{
	if (!bIsDead)
	{
		bIsDead = true;

		if (AController* ControllerInstance = GetController())
		{
			if (ASFBattleGameMode* GM = GetWorld()->GetAuthGameMode<ASFBattleGameMode>())
			{
				GM->RequestRespawn(ControllerInstance);
			}
		}

		Destroy();
	}
}

void ASFCharacter::DieImmediately()
{
	Die(); // 내부적으로 같은 처리
}