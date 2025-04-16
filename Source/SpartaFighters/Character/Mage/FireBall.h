#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FireBall.generated.h"

class UProjectileMovementComponent;
class USphereComponent;

UCLASS()
class SPARTAFIGHTERS_API AFireBall : public AActor
{
	GENERATED_BODY()
	
public:	
	AFireBall();

	UPROPERTY(VisibleDefaultsOnly, Category = "Components")
	USphereComponent* CollisionComponent;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UStaticMeshComponent* FireBallMesh;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UProjectileMovementComponent* ProjectileMovement;

	UFUNCTION()
	void OnHit
	(
		UPrimitiveComponent* HitComponent, 
		AActor* OtherActor, 
		UPrimitiveComponent* OtherComponent, 
		FVector NormalImpulse, 
		const FHitResult& Hit
	);

	void DestroyFireBall();
	void FireInDirection(const FVector& CastDirection);
	void SetFireBallSpeed(const float SpeedInput);
	void SetFireBallDamage(float DamageInput) { FireBallDamage = DamageInput; }

	void InitFireBall(const float SpeedInput, const float Damage);

protected:
	virtual void BeginPlay() override;


private:
	float FireBallDamage;

	FTimerHandle DestroyLazyTimer;

//public:	
//	virtual void Tick(float DeltaTime) override;

};
