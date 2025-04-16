

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AI/Boss/AIMagicDamageArea.h"
#include "AIBossCharacter.generated.h"

class UNiagaraComponent;
class UNiagaraSystem;
class UBoxComponent;
class USphereComponent;

UCLASS()
class SPARTAFIGHTERS_API AAIBossCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AAIBossCharacter();

protected:
	virtual void BeginPlay() override;

	//------------BreathAttack---------------

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	UBoxComponent* DamageBox;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	FVector DamageBoxExtent;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	float DamagePerTick;

	FTimerHandle DamageTickTimer;

	FTimerHandle DamageDurationTimer;

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

	virtual float PlayAnimMontage(
		UAnimMontage* AnimMontage,
		float InPlayRate = 1.f,
		FName StartSectionName = NAME_None
	) override;


	UFUNCTION(NetMulticast, Reliable)
	void Multicast_PlayBossAnim(UAnimMontage* Montage);


	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

};
