#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Character/Stats/StatusContainerInterface.h"
#include "DataTable/SkillDataRow.h"

#include "Character/AttackSystem/HandleAttack.h"

#include "SFCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UMovementInputComponent;
class ASFPlayerController;

class UStatusContainerComponent;
class UStatusComponent;
class UStateComponent;
class USkillComponent;

struct FInputActionValue;
struct FSkillDataRow;

class UInputMappingContext;
class UInputAction;

class UAnimMontage;
class UNiagaraSystem;
UCLASS()
class SPARTAFIGHTERS_API ASFCharacter : public ACharacter//, public IStatusContainerInterface
{
	GENERATED_BODY()

public:
	ASFCharacter();
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stat")
	TObjectPtr<UStatusContainerComponent> StatusContainerComponent;
	//virtual UStatusContainerComponent* GetStatusContainerComponent() const override;
	TObjectPtr<UStatusComponent> StatusComponent;
	TObjectPtr<UStateComponent> StateComponent;
	TObjectPtr<USkillComponent> SkillComponent;

	UFUNCTION(BlueprintCallable)
	UStateComponent* GetStateComponent();

protected:
	virtual void BeginPlay() override;

public:
	virtual void InitCharacter();

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UDataTable* SkillDataTable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UDataTable* ItemSkillDataTable;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UAnimMontage> OnDamageMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UAnimMontage> GuardMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UParticleSystem> HitEffect;

protected:
	// TO DO : Seperate Componenets
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Movement
	void Move(const FInputActionValue& Value);

	void StartJump();
	void StopJump();

	void Landed(const FHitResult& Hit) override;

	void DodgePressed();
	void DodgeReleased();

	void CrouchPressed();
	void CrouchReleased();

	// Ability
	void AttackPressed();
	void AttackReleased();

	void SkillAttackPressed();
	void SkillAttackReleased();

	void GuardPressed();
	void GuardReleased();

	void InteractionPressed(); // TO DO : Equip Item

	void SettingPressed();	// TO DO : Show Setting UI


	// TO DO : Seperate Componet
	virtual float TakeDamage(
		float DamageAmount,
		struct FDamageEvent const& DamageEvent,
		AController* EventInstigator,
		AActor* DamageCauser) override;

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_TakeDamageOnServer(const float Damage, const FDamageEvent& DamageEvent, AActor* DamageCauser);

	UFUNCTION()
	void OnHPChanged(AActor* AffectedActor, float HP);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_SpawnHitEffect(const FVector& Location, const FRotator& Rotation);

	//UFUNCTION()
	//void OnCharacterDead();

public:
	virtual void AttackTrace();	// For AnimNotify
	UFUNCTION(Server, Reliable)
	void Server_SpawnFireBall();
	void SpawnFireBall();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class AFireBall> FireballClass;

private:
	bool bIsItemEquipped;

public:
	UFUNCTION(BlueprintCallable)
	void Die();

	UFUNCTION(BlueprintCallable)
	void DieImmediately();

	void RequestRespawn();

	//UFUNCTION(NetMulticast, Unreliable)
	//void Multicast_PlayHeatEffect();

protected:
	bool bIsDead = false;

	UPROPERTY(EditDefaultsOnly, Category = "Reference");
	TObjectPtr<AController> CachedController;

	FTimerHandle RespawnTimer;

	UPROPERTY(EditDefaultsOnly, Category = "Effects");
	UNiagaraSystem* DeathExplosionEffect;

	UFUNCTION(NetMulticast,Unreliable)
	void Multicast_PlayDeathEffect();

};
