#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameplayTagContainer.h"
#include "AI/Boss/AIMagicDamageArea.h"
#include "AIBossCharacter.generated.h"

class UNiagaraComponent;
class UNiagaraSystem;
class UStatusComponent;
class UBoxComponent;
class USphereComponent;

UCLASS()
class SPARTAFIGHTERS_API AAIBossCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AAIBossCharacter();

	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent,
		AController* EventInstigator, AActor* DamageCauser) override;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStatusComponent* StatusComponent;

	//UFUNCTION(NetMulticast, Reliable)
	//void Multicast_PlayTakeDamageAnimMontage();

	//UFUNCTION(NetMulticast, Reliable)
	//void Multicast_SpawnHitEffect(const FVector& Location, const FRotator& Rotation);

	//UPROPERTY(EditAnywhere, Category = "Effects")
	//UParticleSystem* HitEffect;

	//UPROPERTY(EditAnywhere, Category = "Animation")
	//UAnimMontage* OnDamageMontage;



	UFUNCTION()
	void OnHPChanged(AActor* AffectedActor, float HP);

	UFUNCTION()
	void Die();

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_PlayDeathEffect();

	UPROPERTY(EditAnywhere, Category = "Effects")
	UNiagaraSystem* DeathExplosionEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimMontage* DeathMontage;

	FTimerHandle TimerHandle_DestroyBoss;

	UPROPERTY(Replicated, VisibleAnywhere, Category = "Status")
	bool bIsDead = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
	UMaterialInterface* DissolveMaterial;

	FTimerHandle DissolveTimerHandle;

	UPROPERTY(ReplicatedUsing = OnRep_DissolveProgress)
	float DissolveProgress;

	UFUNCTION()
	void OnRep_DissolveProgress();

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_StartDissolveEffect();

	void StartDissolveEffect();

	FTimerHandle TimerHandle_EnableRagdoll;

	void EnableRagdoll();

public:
	UPROPERTY(ReplicatedUsing = OnRep_BossTags, EditAnywhere, Category = "Boss Phase")
	FGameplayTagContainer BossCharacterTags;

	UPROPERTY(EditAnywhere, Category = "Boss Phase")
	FGameplayTag Phase1Tag;

	UPROPERTY(EditAnywhere, Category = "Boss Phase")
	FGameplayTag Phase2Tag;

	UPROPERTY(EditAnywhere, Category = "Boss Phase")
	FGameplayTag Phase3Tag;


	UPROPERTY(EditAnywhere, Category = "Effects")
	UMaterialInterface* Phase2Material;

	UPROPERTY(EditAnywhere, Category = "Effects")
	UNiagaraSystem* Phase2AuraSystem;

protected:
	UFUNCTION()
	void OnRep_BossTags();

	UFUNCTION(Server, Reliable)
	void Server_ChangePhase(const FGameplayTag& NewPhaseTag);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_ApplyPhaseEffects(const FGameplayTag& PhaseTag);

	//------------BreathAttack---------------

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	UBoxComponent* DamageBox;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	FVector DamageBoxExtent;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	float DamagePerTick;

	FTimerHandle DamageTickTimer;

	FTimerHandle DamageDurationTimer;

	FTimerHandle DelayStartTimer;

	int32 DamageTickCount;

	int32 MaxDamageTicks;

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_StartFireOnServer();

public:
	UFUNCTION(BlueprintCallable, Category = "Boss Skills")
	void StartFire();

	void ApplyBoxDamage();

	//---------------------------------------

	//---------ClawAttack--------------------

protected:
	UPROPERTY(VisibleAnywhere, Category = "Combat")
	USphereComponent* AreaAttackSphere;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	float AreaDamage;

	FTimerHandle AreaAttackDelayHandle;

	void DelayedAreaAttack();

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_AreaAttackOnServer();

public:
	UFUNCTION(BlueprintCallable, Category = "Boss Skills")
	void AreaAttack();

	//---------------------------------------

	//---------MagicAttack-------------------

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	UNiagaraSystem* MagicAttackFX;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	UNiagaraSystem* MagicCircleFX;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	TSubclassOf<AAIMagicDamageArea> MagicDamageAreaClass;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	float MagicRadius;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	float MagicHeight;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	float MagicDamage;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	float WarningDelay = 0.5f;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	float MagicCircleDuration = 6.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	float MagicAttackDuration = 5.0f;

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_CastMagicEffect(FVector Location);

public:
	UFUNCTION(BlueprintCallable, Category = "Boss Skills")
	void CastMagicAttack(const FVector& TargetLocation);


	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	int32 MaxRandomAttacks = 3;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	float MagicAttackInterval = 1.5f;

	FTimerHandle AttackSequenceTimer;

	FTimerHandle DamageDelayHandle;

	int32 CurrentAttackCount = 0;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	float MagicDuration = 5.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	float MagicTickInterval = 1.0f;

	UFUNCTION(BlueprintCallable, Category = "Boss Skills")
	void StartMagicAttackSequence();

	void ExecuteRandomAttack();

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_PreMagicEffect(FVector Location);

	//---------------------------------------


public:
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	UAnimSequence* AttackSequence;

	UPROPERTY(Replicated)
	bool bIsPlaying = false;

	UFUNCTION()
	void PlayAttackAnimation(UAnimSequence* AnimSequence);

	UFUNCTION(Server, Reliable)
	void Server_PlayAttack(UAnimSequence* AnimSequence);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_PlayAttack(UAnimSequence* AnimSequence);

	UPROPERTY(ReplicatedUsing = OnRep_IsPlayingAttack, Transient)
	bool bIsPlayingAttack = false;

	UFUNCTION()
	void OnRep_IsPlayingAttack();

	UPROPERTY(Replicated)
	UAnimSequence* CurrentAttackSequence;

	//virtual float PlayAnimMontage(
	//	UAnimMontage* AnimMontage,
	//	float InPlayRate = 1.f,
	//	FName StartSectionName = NAME_None
	//) override;


	//UFUNCTION(NetMulticast, Reliable)
	//void Multicast_PlayBossAnim(UAnimMontage* Montage);


	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

};