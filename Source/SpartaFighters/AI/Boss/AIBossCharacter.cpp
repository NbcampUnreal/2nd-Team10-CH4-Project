


#include "AI/Boss/AIBossCharacter.h"
#include "AI/AICharacterController.h"
#include "NiagaraSystem.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Character/Components/StatusComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "Components/CapsuleComponent.h"
#include "BrainComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"

AAIBossCharacter::AAIBossCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	StatusComponent = CreateDefaultSubobject<UStatusComponent>(TEXT("StatusComponent"));

	DamageBoxExtent = FVector(2000, 400, 50);

	DamageBox = CreateDefaultSubobject<UBoxComponent>(TEXT("DamageBox"));
	DamageBox->SetupAttachment(GetMesh(), TEXT("Head"));
	DamageBox->SetBoxExtent(DamageBoxExtent);
	DamageBox->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	DamageBox->SetSimulatePhysics(false);
	DamageBox->SetEnableGravity(false);

	DamagePerTick = 20.0f;
	DamageTickCount = 0;
	MaxDamageTicks = 7;

#if WITH_EDITOR
	DamageBox->SetHiddenInGame(false);
	DamageBox->SetVisibility(true);

	DamageBox->ShapeColor = FColor::Red;
#endif


	AreaAttackSphere = CreateDefaultSubobject<USphereComponent>(TEXT("AreaAttackSphere"));
	AreaAttackSphere->SetupAttachment(RootComponent);
	AreaAttackSphere->SetSphereRadius(800.0f);
	AreaAttackSphere->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	AreaAttackSphere->SetSimulatePhysics(false);
	AreaAttackSphere->SetEnableGravity(false);

	AreaDamage = 20.f;


	MagicRadius = 500.0f;
	MagicHeight = 1000.0f;
	MagicDamage = 30.0f;
	WarningDelay = 0.5f;
	MagicCircleDuration = 6.0f;
	MagicAttackDuration = 5.0f;
	MaxRandomAttacks = 3;
	MagicAttackInterval = 1.5f;
	CurrentAttackCount = 0;
	MagicDuration = 5.0f;
	MagicTickInterval = 1.0f;



	bReplicates = true;
	SetNetUpdateFrequency(60.0f);
	SetMinNetUpdateFrequency(30.0f);

	GetMesh()->SetIsReplicated(true);

	if (GetCharacterMovement())
	{
		GetCharacterMovement()->bUseControllerDesiredRotation = true;
		GetCharacterMovement()->bOrientRotationToMovement = false;
	}

	Phase1Tag = FGameplayTag::RequestGameplayTag("Boss.Phase.1");
	Phase2Tag = FGameplayTag::RequestGameplayTag("Boss.Phase.2");
	Phase3Tag = FGameplayTag::RequestGameplayTag("Boss.Phase.3");
	BossCharacterTags.AddTag(Phase1Tag);
}

void AAIBossCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (StatusComponent)
	{
		StatusComponent->OnDeath.AddDynamic(this, &AAIBossCharacter::Die);
		StatusComponent->OnHealthChanged.AddDynamic(this, &AAIBossCharacter::OnHPChanged);
	}

	if (!GetMesh()->DoesSocketExist("Head"))
	{
		UE_LOG(LogTemp, Error, TEXT("[BeginPlay] Head socket MISSING!"));
	}

	if (DamageBox)
	{
		FVector BoxLocation = DamageBox->GetComponentLocation();
		FVector BoxExtent = DamageBox->GetScaledBoxExtent();
	}
}

float AAIBossCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent,
	AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if (!HasAuthority()) return DamageAmount;

	if (StatusComponent)
	{
		StatusComponent->ModifyStatus(EStatusType::CurHP, -DamageAmount);
		float CurrentHP = StatusComponent->GetStatusValue(EStatusType::CurHP);
		float MaxHP = StatusComponent->GetStatusValue(EStatusType::MaxHP);
		UE_LOG(LogTemp, Warning, TEXT("Boss Damaged - Current HP: %.2f"), CurrentHP);

		if (HasAuthority() && BossCharacterTags.HasTag(Phase1Tag))
		{
			float HealthPercent = CurrentHP / MaxHP;
			if (HealthPercent <= 0.5f)
			{
				Server_ChangePhase(Phase2Tag);
			}
		}
	}


	//Multicast_PlayTakeDamageAnimMontage();

	//if (const FSkillDamageEvent* CustomEvent = static_cast<const FSkillDamageEvent*>(&DamageEvent))
	//{
	//	float knockbackPower = CustomEvent->KnockBackPower;
	//	FVector Direction = (GetActorLocation() - DamageCauser->GetActorLocation()).GetSafeNormal();
	//	LaunchCharacter(Direction * knockbackPower, true, true);
	//}

	//if (const FPointDamageEvent* PointDamageEvent = static_cast<const FPointDamageEvent*>(&DamageEvent))
	//{
	//	Multicast_SpawnHitEffect(PointDamageEvent->HitInfo.ImpactPoint,
	//		PointDamageEvent->ShotDirection.Rotation());
	//}

	return DamageAmount;
}

//void AAIBossCharacter::Multicast_PlayTakeDamageAnimMontage_Implementation()
//{
//	if (OnDamageMontage)
//	{
//		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
//		if (AnimInstance)
//		{
//			AnimInstance->Montage_Play(OnDamageMontage);
//		}
//	}
//}
//
//void AAIBossCharacter::Multicast_SpawnHitEffect_Implementation(const FVector& Location, const FRotator& Rotation)
//{
//	if (!HitEffect) return;
//
//	UGameplayStatics::SpawnEmitterAtLocation(
//		GetWorld(),
//		HitEffect,
//		Location,
//		Rotation,
//		true
//	);
//}

void AAIBossCharacter::OnHPChanged(AActor* AffectedActor, float HP)
{
	UE_LOG(LogTemp, Warning, TEXT("Boss HP Changed : %.2f"), HP);
}

void AAIBossCharacter::Die()
{
	if (!HasAuthority() || bIsDead) return;

	bIsDead = true;

	if (AAICharacterController* AIController = Cast<AAICharacterController>(GetController()))
	{
		if (AIController->BrainComponent)
		{
			AIController->BrainComponent->StopLogic("Boss Died");
		}
	}

	Multicast_StartDissolveEffect();

	if (DeathMontage)
	{
		Multicast_PlayDeathEffect();
	}

	GetWorldTimerManager().SetTimer(
		TimerHandle_EnableRagdoll,
		this,
		&AAIBossCharacter::EnableRagdoll,
		1.0f,
		false
	);

	GetWorldTimerManager().SetTimer(
		TimerHandle_DestroyBoss,
		[this]() { Destroy(); },
		6.0f, false
	);
}

void AAIBossCharacter::OnRep_DissolveProgress()
{
	if (UMaterialInstanceDynamic* DynMaterial = Cast<UMaterialInstanceDynamic>(GetMesh()->GetMaterial(0)))
	{
		DynMaterial->SetScalarParameterValue("Dissolve", DissolveProgress);
	}
}

void AAIBossCharacter::Multicast_StartDissolveEffect_Implementation()
{
	StartDissolveEffect();
}

void AAIBossCharacter::StartDissolveEffect()
{
	if (!DissolveMaterial) return;

	UMaterialInstanceDynamic* DynMaterial = UMaterialInstanceDynamic::Create(DissolveMaterial, this);

	GetMesh()->SetMaterial(0, DynMaterial);

	DynMaterial->SetScalarParameterValue("Dissolve", -1.0f);

	DissolveProgress = -1.0f;
	GetWorldTimerManager().SetTimer(
		DissolveTimerHandle,
		[this, DynMaterial]() // DynMaterial 캡처 추가
		{
			DissolveProgress += 0.016f;

			// 서버/싱글플레이: 직접 파라미터 업데이트
			if (HasAuthority())
			{
				DynMaterial->SetScalarParameterValue("Dissolve", DissolveProgress);
			}

			// 클라이언트: 리플리케이션으로 처리
			if (DissolveProgress >= 1.0f)
			{
				GetWorldTimerManager().ClearTimer(DissolveTimerHandle);
			}
		},
		0.04f,
		true
	);
}

void AAIBossCharacter::EnableRagdoll()
{
	USkeletalMeshComponent* SkeletalMesh = GetMesh();
	if (!SkeletalMesh) return;

	SkeletalMesh->SetCollisionProfileName(TEXT("Ragdoll"));
	SkeletalMesh->SetSimulatePhysics(true);

	if (GetCharacterMovement())
	{
		GetCharacterMovement()->DisableMovement();
	}

	if (GetCapsuleComponent())
	{
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void AAIBossCharacter::OnRep_BossTags()
{
	if (BossCharacterTags.HasTag(Phase2Tag))
	{
		GetMesh()->SetMaterial(0, Phase2Material);
	}
}

void AAIBossCharacter::Server_ChangePhase_Implementation(const FGameplayTag& NewPhaseTag)
{
	BossCharacterTags.RemoveTag(Phase1Tag);
	BossCharacterTags.RemoveTag(Phase2Tag);
	BossCharacterTags.RemoveTag(Phase3Tag);

	BossCharacterTags.AddTag(NewPhaseTag);

	Multicast_ApplyPhaseEffects(NewPhaseTag);
}

void AAIBossCharacter::Multicast_ApplyPhaseEffects_Implementation(const FGameplayTag& PhaseTag)
{
	if (PhaseTag == Phase2Tag)
	{
		GetMesh()->SetMaterial(0, Phase2Material);

		if (Phase2AuraSystem)
		{
			UNiagaraFunctionLibrary::SpawnSystemAttached(
				Phase2AuraSystem,
				GetMesh(),
				NAME_None,
				FVector::ZeroVector,
				FRotator::ZeroRotator,
				EAttachLocation::SnapToTarget,
				true
			);
		}
	}
}

void AAIBossCharacter::Multicast_PlayDeathEffect_Implementation()
{
	//if (DeathExplosionEffect)
	//{
	//	UNiagaraFunctionLibrary::SpawnSystemAtLocation(
	//		GetWorld(),
	//		DeathExplosionEffect,
	//		GetActorLocation(),
	//		GetActorRotation()
	//	);
	//}

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && DeathMontage)
	{
		AnimInstance->Montage_Play(DeathMontage);
	}
}


void AAIBossCharacter::Multicast_StartFireOnServer_Implementation()
{
	//DrawDebugBox(
	//	GetWorld(),
	//	DamageBox->GetComponentLocation(),
	//	DamageBox->GetScaledBoxExtent(),
	//	DamageBox->GetComponentQuat(),
	//	FColor::Red,
	//	false,
	//	1.0f,
	//	0,
	//	2.0f
	//);
}

void AAIBossCharacter::StartFire()
{
	if (HasAuthority())
	{
		GetWorld()->GetTimerManager().SetTimer(
			DelayStartTimer,
			[this]()
			{
				GetWorld()->GetTimerManager().SetTimer(DamageTickTimer, this, &AAIBossCharacter::ApplyBoxDamage, 0.2f, true);

				GetWorld()->GetTimerManager().SetTimer(
					DamageDurationTimer,
					[this]()
					{
						GetWorld()->GetTimerManager().ClearTimer(DamageTickTimer);
					},
					1.5f,
					false
				);

				Multicast_StartFireOnServer();
			},
			1.0f,
			false
		);
	}
}

void AAIBossCharacter::ApplyBoxDamage()
{
	FVector BoxLoc = DamageBox->GetComponentLocation();
	FVector BoxExtent = DamageBox->GetScaledBoxExtent();

	TArray<AActor*> OverlappingActors;
	DamageBox->GetOverlappingActors(OverlappingActors, ACharacter::StaticClass());

	//DrawDebugBox(
	//	GetWorld(),
	//	DamageBox->GetComponentLocation(),
	//	DamageBox->GetScaledBoxExtent(),
	//	DamageBox->GetComponentQuat(),
	//	FColor::Red,
	//	false,
	//	1.0f,
	//	0,
	//	2.0f
	//);

	for (AActor* Actor : OverlappingActors)
	{
		if (Actor == this || Actor == nullptr) continue;

		UGameplayStatics::ApplyDamage(
			Actor,
			DamagePerTick,
			GetController(),
			this,
			UDamageType::StaticClass()
		);
	}
}

void AAIBossCharacter::Multicast_AreaAttackOnServer_Implementation()
{
	//DrawDebugBox(
	//	GetWorld(),
	//	DamageBox->GetComponentLocation(),
	//	DamageBox->GetScaledBoxExtent(),
	//	DamageBox->GetComponentQuat(),
	//	FColor::Red,
	//	false,
	//	1.0f,
	//	0,
	//	2.0f
	//);
}

void AAIBossCharacter::AreaAttack()
{
	if (!HasAuthority()) return;

	GetWorld()->GetTimerManager().SetTimer(
		AreaAttackDelayHandle,
		this,
		&AAIBossCharacter::DelayedAreaAttack,
		1.5f,
		false
	);
}

void AAIBossCharacter::DelayedAreaAttack()
{
	TArray<AActor*> OverlappingActors;
	AreaAttackSphere->GetOverlappingActors(OverlappingActors, ACharacter::StaticClass());

	for (AActor* Actor : OverlappingActors)
	{
		if (Actor == this || Actor == nullptr) continue;

		UGameplayStatics::ApplyDamage(
			Actor,
			AreaDamage,
			GetController(),
			this,
			UDamageType::StaticClass()
		);
	}

	//DrawDebugSphere(
	//	GetWorld(),
	//	GetActorLocation(),
	//	AreaAttackSphere->GetScaledSphereRadius(),
	//	32,
	//	FColor::Red,
	//	false,
	//	2.0f
	//);
}


void AAIBossCharacter::Multicast_CastMagicEffect_Implementation(FVector Location)
{
	if (!MagicAttackFX) return;

	UNiagaraComponent* AttackComp = UNiagaraFunctionLibrary::SpawnSystemAtLocation(
		GetWorld(),
		MagicAttackFX,
		Location,
		FRotator::ZeroRotator,
		FVector(1.0f),
		true,
		true,
		ENCPoolMethod::AutoRelease
	);

	if (AttackComp)
	{
		FTimerHandle AttackTimer;
		GetWorld()->GetTimerManager().SetTimer(
			AttackTimer,
			[AttackComp]()
			{
				if (AttackComp && AttackComp->IsActive())
				{
					AttackComp->Deactivate();
					AttackComp->DestroyComponent();
				}
			},
			MagicAttackDuration,
			false
		);
	}
}

void AAIBossCharacter::CastMagicAttack(const FVector& TargetLocation)
{
	if (!HasAuthority()) return;

	Multicast_PreMagicEffect(TargetLocation);

	GetWorld()->GetTimerManager().SetTimer(
		DamageDelayHandle,
		[this, TargetLocation]()
		{
			if (MagicDamageAreaClass)
			{
				FActorSpawnParameters SpawnParams;
				SpawnParams.Owner = this;

				AAIMagicDamageArea* DamageArea = GetWorld()->SpawnActor<AAIMagicDamageArea>(
					MagicDamageAreaClass,
					TargetLocation,
					FRotator::ZeroRotator,
					SpawnParams
				);

				if (DamageArea && DamageArea->DamageCapsule)
				{
					DamageArea->DamageCapsule->SetCapsuleSize(MagicRadius, MagicHeight / 2);
				}
			}

			Multicast_CastMagicEffect(TargetLocation);
		},
		WarningDelay,
		false
	);
}

void AAIBossCharacter::StartMagicAttackSequence()
{
	if (!HasAuthority())
	{
		return;
	}

	CurrentAttackCount = 0;
	GetWorld()->GetTimerManager().SetTimer(
		AttackSequenceTimer,
		this,
		&AAIBossCharacter::ExecuteRandomAttack,
		MagicAttackInterval,
		false
	);
}

void AAIBossCharacter::ExecuteRandomAttack()
{
	if (CurrentAttackCount >= MaxRandomAttacks)
	{
		GetWorld()->GetTimerManager().ClearTimer(AttackSequenceTimer);
		return;
	}

	TArray<AActor*> PossibleTargets;
	TArray<AActor*> AllCharacters;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACharacter::StaticClass(), AllCharacters);

	for (AActor* Actor : AllCharacters)
	{
		if (Actor != this && IsValid(Actor))
		{
			ACharacter* Char = Cast<ACharacter>(Actor);
			if (Char)
			{
				PossibleTargets.Add(Actor);
			}
		}
	}

	if (PossibleTargets.Num() > 0)
	{
		int32 RandomIndex = FMath::RandRange(0, PossibleTargets.Num() - 1);
		FVector AttackLocation = PossibleTargets[RandomIndex]->GetActorLocation();

		FHitResult HitResult;
		if (GetWorld()->LineTraceSingleByChannel(HitResult,
			AttackLocation + FVector(0, 0, 1000),
			AttackLocation - FVector(0, 0, 1000),
			ECC_Visibility))
		{
			AttackLocation = HitResult.Location;
		}

		//DrawDebugCapsule(
		//	GetWorld(),
		//	AttackLocation,
		//	MagicHeight / 2,
		//	MagicRadius,
		//	FQuat::Identity,
		//	FColor::Green,
		//	false,
		//	2.0f,
		//	0,
		//	3.0f
		//);

		CastMagicAttack(AttackLocation);
		UE_LOG(LogTemp, Warning, TEXT("Attacking location: %s"), *AttackLocation.ToString());
	}

	CurrentAttackCount++;
}

void AAIBossCharacter::Multicast_PreMagicEffect_Implementation(FVector Location)
{
	if (!MagicCircleFX) return;

	UNiagaraComponent* CircleComp = UNiagaraFunctionLibrary::SpawnSystemAtLocation(
		GetWorld(),
		MagicCircleFX,
		Location,
		FRotator::ZeroRotator,
		FVector(1.0f),
		true,
		true,
		ENCPoolMethod::AutoRelease
	);

	if (CircleComp)
	{
		FTimerHandle CircleTimer;
		GetWorld()->GetTimerManager().SetTimer(
			CircleTimer,
			[CircleComp]()
			{
				if (CircleComp && CircleComp->IsActive())
				{
					CircleComp->Deactivate();
					CircleComp->DestroyComponent();
				}
			},
			MagicCircleDuration,
			false
		);
	}

	//DrawDebugCapsule(
	//	GetWorld(),
	//	Location,
	//	MagicHeight / 2,
	//	MagicRadius,
	//	FQuat::Identity,
	//	FColor::Yellow,
	//	false,
	//	WarningDelay,
	//	0,
	//	3.0f
	//);
}

void AAIBossCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AAIBossCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AAIBossCharacter::PlayAttackAnimation(UAnimSequence* AnimSequence)
{
	if (bIsPlayingAttack || !AnimSequence) return;

	CurrentAttackSequence = AnimSequence;
	bIsPlayingAttack = true;

	if (HasAuthority())
	{
		Multicast_PlayAttack(AnimSequence);
	}
	else
	{
		Server_PlayAttack(AnimSequence);
	}
}

void AAIBossCharacter::Server_PlayAttack_Implementation(UAnimSequence* AnimSequence)
{
	UE_LOG(LogTemp, Warning, TEXT("[Server_PlayAttack] server called. AnimSequence: %s"), *GetNameSafe(AnimSequence));
	CurrentAttackSequence = AnimSequence;
	bIsPlayingAttack = true;
	Multicast_PlayAttack(AnimSequence);
}

void AAIBossCharacter::Multicast_PlayAttack_Implementation(UAnimSequence* AnimSequence)
{
	if (AnimSequence && GetMesh() && GetMesh()->GetAnimInstance())
	{
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (AnimInstance)
		{
			AnimInstance->PlaySlotAnimationAsDynamicMontage(
				AnimSequence,
				"DefaultSlot",
				0.2f,
				0.2f,
				1.0f
			);
		}
	}

	bIsPlayingAttack = false;
}

void AAIBossCharacter::OnRep_IsPlayingAttack()
{
	if (bIsPlayingAttack && CurrentAttackSequence)
	{
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (AnimInstance && CurrentAttackSequence)
		{
			AnimInstance->PlaySlotAnimationAsDynamicMontage(
				CurrentAttackSequence,
				"DefaultSlot",
				0.2f,
				0.2f,
				1.0f
			);
		}
	}
}


//float AAIBossCharacter::PlayAnimMontage(
//	UAnimMontage* AnimMontage,
//	float InPlayRate,
//	FName StartSectionName
//)
//{
//	float Duration = Super::PlayAnimMontage(AnimMontage, InPlayRate, StartSectionName);
//
//	UE_LOG(LogTemp, Warning, TEXT("Montage duration: %f"), Duration);
//	return Duration;
//}
//
//void AAIBossCharacter::Multicast_PlayBossAnim_Implementation(UAnimMontage* Montage)
//{
//	if (!HasAuthority() && GetMesh()->GetAnimInstance())
//	{
//		PlayAnimMontage(Montage);
//	}
//}

void AAIBossCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AAIBossCharacter, bIsPlayingAttack);
	DOREPLIFETIME(AAIBossCharacter, bIsDead);
	DOREPLIFETIME(AAIBossCharacter, CurrentAttackSequence);
	DOREPLIFETIME(AAIBossCharacter, DissolveProgress);
	DOREPLIFETIME(AAIBossCharacter, BossCharacterTags);
}





