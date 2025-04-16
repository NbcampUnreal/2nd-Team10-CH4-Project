


#include "AI/Boss/AIBossCharacter.h"
#include "NiagaraSystem.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"

AAIBossCharacter::AAIBossCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	//FlameFX = CreateDefaultSubobject<UNiagaraComponent>(TEXT("FlameFX"));
	//FlameFX->SetupAttachment(GetMesh(), TEXT("Head"));
	//FlameFX->bAutoActivate = false;

	//static ConstructorHelpers::FObjectFinder<UNiagaraSystem> Asset(TEXT("/Game/FourEvilDragonsPBR/FX_FlameThrower"));
	//if (Asset.Succeeded()) FlameEffect = Asset.Object;

	//FlameFX->SetAsset(FlameEffect);

	DamageBoxExtent = FVector(2000, 400, 50);

	DamageBox = CreateDefaultSubobject<UBoxComponent>(TEXT("DamageBox"));
	DamageBox->SetupAttachment(GetMesh(), TEXT("Head"));
	DamageBox->SetBoxExtent(DamageBoxExtent);
	DamageBox->SetCollisionProfileName(TEXT("OverlapAllDynamic"));

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

	AreaDamage = 20.f;


	MagicRadius = 500.0f;
	MagicHeight = 1000.0f;
	MagicDamage = 30.0f;

	bReplicates = true;
	SetNetUpdateFrequency(60.0f);
	SetMinNetUpdateFrequency(30.0f);

	GetMesh()->SetIsReplicated(true);

	if (GetCharacterMovement())
	{
		GetCharacterMovement()->bUseControllerDesiredRotation = true;
		GetCharacterMovement()->bOrientRotationToMovement = false;
	}
}

void AAIBossCharacter::BeginPlay()
{
	Super::BeginPlay();

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

void AAIBossCharacter::Multicast_StartFireOnServer_Implementation()
{
	DrawDebugBox(
		GetWorld(),
		DamageBox->GetComponentLocation(),
		DamageBox->GetScaledBoxExtent(),
		DamageBox->GetComponentQuat(),
		FColor::Red,
		false,
		1.0f,
		0,
		2.0f
	);
}

void AAIBossCharacter::StartFire()
{
	if (HasAuthority())
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
	}
}

void AAIBossCharacter::ApplyBoxDamage()
{
	FVector BoxLoc = DamageBox->GetComponentLocation();
	FVector BoxExtent = DamageBox->GetScaledBoxExtent();

	TArray<AActor*> OverlappingActors;
	DamageBox->GetOverlappingActors(OverlappingActors, ACharacter::StaticClass());

	DrawDebugBox(
		GetWorld(),
		DamageBox->GetComponentLocation(),
		DamageBox->GetScaledBoxExtent(),
		DamageBox->GetComponentQuat(),
		FColor::Red,
		false,
		1.0f,
		0,
		2.0f
	);

	for (AActor* Actor : OverlappingActors)
	{
		UE_LOG(LogTemp, Warning, TEXT("[ApplyDamage] Damaging: %s"), *GetNameSafe(Actor));
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
	DrawDebugBox(
		GetWorld(),
		DamageBox->GetComponentLocation(),
		DamageBox->GetScaledBoxExtent(),
		DamageBox->GetComponentQuat(),
		FColor::Red,
		false,
		1.0f,
		0,
		2.0f
	);
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
		UGameplayStatics::ApplyDamage(
			Actor,
			AreaDamage,
			GetController(),
			this,
			UDamageType::StaticClass()
		);
	}

	DrawDebugSphere(
		GetWorld(),
		GetActorLocation(),
		AreaAttackSphere->GetScaledSphereRadius(),
		32,
		FColor::Red,
		false,
		2.0f
	);
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

		DrawDebugCapsule(
			GetWorld(),
			AttackLocation,
			MagicHeight / 2,
			MagicRadius,
			FQuat::Identity,
			FColor::Green,
			false,
			2.0f,
			0,
			3.0f
		);

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

	DrawDebugCapsule(
		GetWorld(),
		Location,
		MagicHeight / 2,
		MagicRadius,
		FQuat::Identity,
		FColor::Yellow,
		false,
		WarningDelay,
		0,
		3.0f
	);
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
	if (HasAuthority()) // 서버에서만 실행
	{
		Multicast_PlayAttack(AnimSequence);
	}

	if (!AnimSequence)
	{
		UE_LOG(LogTemp, Error, TEXT("[PlayAttackAnimation] AnimSequence null"));
		return;
	}

	if (!GetMesh()->GetAnimInstance())
	{
		UE_LOG(LogTemp, Error, TEXT("[PlayAttackAnimation] AnimInstance null"));
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("[PlayAttackAnimation] anim play start: %s"), *GetNameSafe(AnimSequence));

	GetMesh()->GetAnimInstance()->PlaySlotAnimationAsDynamicMontage(
		AnimSequence,
		"DefaultSlot",
		0.5f, // Blend In 시간 증가
		0.5f, // Blend Out 시간 증가
		1.0f,
		1
	);

	bIsPlaying = true;

	FOnMontageEnded OnMontageEnded;
	OnMontageEnded.BindLambda([this](UAnimMontage* Montage, bool bInterrupted) {
		UE_LOG(LogTemp, Warning, TEXT("[PlayAttackAnimation] anim end (Interrupted: %d)"),
			(int)bInterrupted);
		bIsPlaying = false;
		});
	GetMesh()->GetAnimInstance()->Montage_SetEndDelegate(OnMontageEnded);

	UE_LOG(LogTemp, Warning, TEXT("Anim Instance: %s"), *GetNameSafe(GetMesh()->GetAnimInstance()));
	UE_LOG(LogTemp, Warning, TEXT("Skeleton: %s"), *GetNameSafe(GetMesh()->GetSkeletalMeshAsset()->GetSkeleton()));
}

void AAIBossCharacter::Server_PlayAttack_Implementation(UAnimSequence* AnimSequence)
{
	UE_LOG(LogTemp, Warning, TEXT("[Server_PlayAttack] server called. AnimSequence: %s"), *GetNameSafe(AnimSequence));
	Multicast_PlayAttack(AnimSequence);
}

void AAIBossCharacter::Multicast_PlayAttack_Implementation(UAnimSequence* AnimSequence)
{
	if (GetMesh() && GetMesh()->GetAnimInstance())
	{
		PlayAttackAnimation(AnimSequence);
	}
}

float AAIBossCharacter::PlayAnimMontage(
	UAnimMontage* AnimMontage,
	float InPlayRate,
	FName StartSectionName
)
{
	float Duration = Super::PlayAnimMontage(AnimMontage, InPlayRate, StartSectionName);

	UE_LOG(LogTemp, Warning, TEXT("Montage duration: %f"), Duration);
	return Duration;
}

void AAIBossCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AAIBossCharacter, bIsPlaying);
}

void AAIBossCharacter::Multicast_PlayBossAnim_Implementation(UAnimMontage* Montage)
{
	if (!HasAuthority() && GetMesh()->GetAnimInstance())
	{
		PlayAnimMontage(Montage);
	}
}




