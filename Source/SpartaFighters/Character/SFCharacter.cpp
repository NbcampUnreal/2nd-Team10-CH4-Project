#include "Character/SFCharacter.h"

#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"

#include "Framework/SFPlayerController.h"
#include "Framework/SFBattleGameMode.h"
#include "Framework/SFCooperativeGameMode.h"

#include "Components/StatusContainerComponent.h"
#include "Components/StatusComponent.h"
#include "Components/StateComponent.h"
#include "Components/SkillComponent.h"
#include "Inventory/SFInventoryComponent.h"

#include "Engine/DamageEvents.h"
#include "Common/SkillDamageEvent.h"
#include "Kismet/GameplayStatics.h"
#include "DataTable/SkillDataRow.h"

#include "Net/UnrealNetwork.h"
#include "Character/Mage/FireBall.h"
#include "NiagaraFunctionLibrary.h"




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
	StatusComponent = CreateDefaultSubobject<UStatusComponent>(TEXT("StatusComponent"));
	StateComponent = CreateDefaultSubobject<UStateComponent>(TEXT("StateComponent"));
	SkillComponent = CreateDefaultSubobject<USkillComponent>(TEXT("SkillComponent"));
	InventoryComponent = CreateDefaultSubobject<USFInventoryComponent>(TEXT("InventoryComponent"));

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

UStateComponent* ASFCharacter::GetStateComponent()
{
	return StateComponent;
}

UStatusComponent* ASFCharacter::GetStatusComponent()
{
	return StatusComponent;
}

void ASFCharacter::BeginPlay()
{
	Super::BeginPlay();

	//StatusContainerComponent->InitStatusComponent(this);
	//StateComponent->Init();
	if (StatusComponent)
	{
		StatusComponent->OnDeath.AddDynamic(this, &ASFCharacter::Die);
		StatusComponent->OnHealthChanged.AddDynamic(this, &ASFCharacter::OnHPChanged);
	}
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

void ASFCharacter::DodgePressed()
{
	// TO DO : Move To SKill Component
	if (StateComponent->IsInAction()) return;
	SkillComponent->HandleInputDodge();
}

void ASFCharacter::DodgeReleased()
{

}


void ASFCharacter::CrouchPressed()
{
	if (!StateComponent->CanCrouch()) return;

	Crouch();
	StateComponent->SetState(ECharacterState::Crouching);
}

void ASFCharacter::CrouchReleased()
{
	UnCrouch();
	StateComponent->SetState(ECharacterState::Idle);
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
	if (GuardMontage)
	{
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (AnimInstance)
		{
			AnimInstance->Montage_Play(GuardMontage);
		}
	}

}

void ASFCharacter::GuardReleased()
{
	if (GuardMontage)
	{
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (AnimInstance)
		{
			AnimInstance->Montage_Stop(0.1f, GuardMontage);
		}
	}
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

void ASFCharacter::Server_SpawnFireBall_Implementation()
{
	SpawnFireBall();
}

void ASFCharacter::SpawnFireBall()
{
	if (!HasAuthority()) return;

	const FVector Forward = GetActorForwardVector();
	const FVector SpawnLocation = GetActorLocation() + Forward * 100.f;
	const FRotator SpawnRotation = GetActorRotation();

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = GetInstigator();

	AFireBall* FireBall = GetWorld()->SpawnActor<AFireBall>(FireballClass, SpawnLocation, SpawnRotation, SpawnParams);
	if (FireBall)
	{
		auto CurrentSkillData = SkillDataTable->FindRow<FSkillDataRow>("IdleSkill", TEXT("SkillLookup"));
		if (!CurrentSkillData || !CurrentSkillData->SkillMontage) return;

		FireBall->InitFireBall(1500, CurrentSkillData->AttackPower);
		UE_LOG(LogTemp, Warning, TEXT("Casting Fire Ball!!"));

		StatusComponent->ModifyStatus(EStatusType::CurMP, -CurrentSkillData->MPCost);
	}
}

// TO DO : Damage Component
float ASFCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	auto hasAuthority = HasAuthority();
	//ensureAlways(hasAuthority);

	if (StatusComponent)
	{
		StatusComponent->ModifyStatus(EStatusType::CurHP, -DamageAmount);
	}

	Multicast_PlayTakeDamageAnimMontage();

	// Knock Back
	if (const FSkillDamageEvent* CustomEvent = static_cast<const FSkillDamageEvent*>(&DamageEvent))
	{
		float knockbackPower = CustomEvent->KnockBackPower;
		FVector Direction = (GetActorLocation() - DamageCauser->GetActorLocation()).GetSafeNormal();
		LaunchCharacter(Direction * knockbackPower, true, true);
	}

	// Damage Effect
	if (const FPointDamageEvent* PointDamageEvent = static_cast<const FPointDamageEvent*>(&DamageEvent))
	{
		Multicast_SpawnHitEffect(PointDamageEvent->HitInfo.ImpactPoint, PointDamageEvent->ShotDirection.Rotation());
	}

	return DamageAmount;
}

void ASFCharacter::Multicast_PlayTakeDamageAnimMontage_Implementation()
{
	if (OnDamageMontage)
	{
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (AnimInstance)
		{
			AnimInstance->Montage_Play(OnDamageMontage);
		}
	}
}


void ASFCharacter::Multicast_SpawnHitEffect_Implementation(const FVector& Location, const FRotator& Rotation)
{
	if (!HitEffect) return;

	UGameplayStatics::SpawnEmitterAtLocation(
		GetWorld(),
		HitEffect,
		Location,
		Rotation,
		true
	);
}

void ASFCharacter::OnHPChanged(AActor* AffectedActor, float HP)
{
	//if (IsLocallyControlled())
	//{
	//	//FString Message = FString::Printf(TEXT("Take Damage : %f"), HP);
	//	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, Message);
	//	UE_LOG(LogTemp, Warning, TEXT("%s의 체력이 %f로 변경되었습니다."), *AffectedActor->GetName(), HP);
	//}
}

void ASFCharacter::Die()
{
	if (!HasAuthority() || bIsDead)
	{
		return;
	}

	bIsDead = true;

	CachedController = GetController();

	Multicast_PlayDeathEffect();

	SetActorEnableCollision(false);
	SetActorHiddenInGame(true);

	if (CachedController)
	{
		SetOwner(CachedController);
		CachedController->UnPossess();
	}

	GetWorldTimerManager().SetTimer(
		RespawnTimer,
		this,
		&ASFCharacter::RequestRespawn,
		2.0f,
		false
	);
}

void ASFCharacter::DieImmediately()
{
	Die();
}

void ASFCharacter::RequestRespawn()
{
	if (!CachedController && IsValid(GetWorld()))
	{
		CachedController = Cast<AController>(GetOwner());
	}

	if (CachedController)
	{

		if (ASFBattleGameMode* BattleGM = GetWorld()->GetAuthGameMode<ASFBattleGameMode>())
		{
			BattleGM->RequestRespawn(CachedController);
		}
		else if (ASFCooperativeGameMode* CoopGM = GetWorld()->GetAuthGameMode<ASFCooperativeGameMode>())
		{
			CoopGM->RequestRespawn(CachedController);
		}

	}

	Destroy();
}


void ASFCharacter::Multicast_PlayDeathEffect_Implementation()
{
	FVector SpawnLocation = GetActorLocation();
	FRotator SpawnRotation = GetActorRotation();
	UE_LOG(LogTemp, Warning, TEXT("MulticcastPlayDeathCalled"));
	if (DeathExplosionEffect)
	{
	UE_LOG(LogTemp, Warning, TEXT("EffectPlayed"));
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			GetWorld(),
			DeathExplosionEffect,
			SpawnLocation,
			SpawnRotation
		);
	}

	/*if (DeathExplosionSound)
	{
		UGameplayStatics::PlaySoundAtLocation(
			this,
			DeathExplosionSound,
			SpawnLocation
		);
	}*/
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

			EnhancedInput->BindAction(SFPlayerController->DodgeAction, ETriggerEvent::Started, this, &ASFCharacter::DodgePressed);
			EnhancedInput->BindAction(SFPlayerController->DodgeAction, ETriggerEvent::Completed, this, &ASFCharacter::DodgeReleased);

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
