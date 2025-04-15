#include "Character/Mage/FireBall.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Common/SKillDamageEvent.h"

AFireBall::AFireBall()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	if (!RootComponent)
	{
		RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("ProjectileSceneComponent"));
	}

	if (!CollisionComponent)
	{
		CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
		CollisionComponent->InitSphereRadius(10.0f);
		CollisionComponent->BodyInstance.SetCollisionProfileName(TEXT("Projectile"));
		RootComponent = CollisionComponent;

		if (GetLocalRole() == ROLE_Authority)
		{
			CollisionComponent->OnComponentHit.AddDynamic(this, &AFireBall::OnHit); // TODO : OnHit Needs
		}
	}

	FireBallMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FireBallMesh"));
	FireBallMesh->SetupAttachment(CollisionComponent);
	FireBallMesh->SetRelativeRotation(FRotator(0.f, 270.f, 0.f));

	if (!ProjectileMovement)
	{
		ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
		ProjectileMovement->SetUpdatedComponent(CollisionComponent);
		ProjectileMovement->InitialSpeed = 1500.0f;
		ProjectileMovement->MaxSpeed = 1500.0f;
		ProjectileMovement->bRotationFollowsVelocity = true;
		ProjectileMovement->bShouldBounce = true;
		ProjectileMovement->Bounciness = 0.3f;
		ProjectileMovement->ProjectileGravityScale = 0.0f;
	}

	FireBallDamage = 10.f;

}

void AFireBall::BeginPlay()
{
	Super::BeginPlay();

	GetWorld()->GetTimerManager().SetTimer(DestroyLazyTimer, this, &AFireBall::DestroyFireBall, 2.f, false);
	
}

void AFireBall::FireInDirection(const FVector& CastDirection)
{
	if (ProjectileMovement)
	{
		ProjectileMovement->Velocity = CastDirection * ProjectileMovement->InitialSpeed;
	}
}

void AFireBall::SetFireBallSpeed(const float SpeedInput)
{
	ProjectileMovement->MaxSpeed = ProjectileMovement->InitialSpeed + SpeedInput;
	ProjectileMovement->Velocity = ProjectileMovement->Velocity.GetSafeNormal() * (ProjectileMovement->MaxSpeed);
}

void AFireBall::InitFireBall(const float SpeedInput, const float Damage)
{
	SetFireBallSpeed(SpeedInput);
	FireBallDamage = Damage;
}

void AFireBall::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	if (!OtherActor || OtherActor == this || OtherActor == GetOwner())
	{
		return; // 자기 자신 또는 발사자면 무시
	}

	if (!HasAuthority()) return;

	UE_LOG(LogTemp, Warning, TEXT("FireBall Hit %s"), *OtherActor->GetName());

	ProjectileMovement->bShouldBounce = false;
	GetWorld()->GetTimerManager().SetTimer(DestroyLazyTimer, this, &AFireBall::DestroyFireBall, 0.3f, false);

	//  커스텀 데미지 이벤트 생성
	FSkillDamageEvent DamageEvent;
	DamageEvent.HitInfo = Hit;
	DamageEvent.ShotDirection = GetActorForwardVector();  // 또는 NormalImpulse.GetSafeNormal()
	DamageEvent.DamageTypeClass = UDamageType::StaticClass();
	DamageEvent.KnockBackPower = 1200.f; // 혹은 FireBallDamage * 계수 등으로 계산 가능

	// ?? 데미지 적용 (TakeDamage으로 직접)
	OtherActor->TakeDamage(
		FireBallDamage,
		DamageEvent,
		GetInstigator() ? GetInstigator()->Controller : nullptr,
		this
	);

	UE_LOG(LogTemp, Warning, TEXT("FireBall Apply Custom Damage %f"), FireBallDamage);

	UE_LOG(LogTemp, Warning, TEXT("FireBall Apply Damage %f"), FireBallDamage);
}

void AFireBall::DestroyFireBall()
{
	Destroy();
}