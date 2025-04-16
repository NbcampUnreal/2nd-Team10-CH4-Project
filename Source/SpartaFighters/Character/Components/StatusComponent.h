

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Character/Stats/FStatusStruct.h"
#include "StatusComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDeathEvent);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHealthChanged, AActor*, OwnerActor, float, NewHP);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnMPChanged, AActor*, OwnerActor, float, NewMP);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAttackPowerChanged, AActor*, OwnerActor, float, NewAttackPower);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SPARTAFIGHTERS_API UStatusComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UStatusComponent();

protected:
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
	FStatusStruct StatusStruct;

public:
	// 초기화
	void InitializeStatus();

	// 값 얻기
	float GetStatusValue(EStatusType Type) const;

	// 값 변경
	void ModifyStatus(EStatusType Type, float Amount);

	// 체력 관련
	void ModifyHP(float Amount);

	// 마나 관련
	void ModifyMP(float Amount);

	UFUNCTION()
	void OnRep_CurHP();

	UFUNCTION()
	void OnRep_CurMP();

	// 공격력 관련
	void ModifyAttackPower(float Amount);
	UFUNCTION()
	void OnRep_AttackPower();

	// 상태 구조 전체 반환
	FStatusStruct GetStruct() const { return StatusStruct; }

	UPROPERTY(BlueprintAssignable)
	FOnHealthChanged OnHealthChanged;

	UPROPERTY(BlueprintAssignable)
	FOnMPChanged OnMPChanged;

	UPROPERTY(BlueprintAssignable)
	FOnDeathEvent OnDeath;

	UPROPERTY(BlueprintAssignable)
	FOnAttackPowerChanged OnAttackPowerChanged;

private:

	UPROPERTY(ReplicatedUsing = OnRep_CurHP)
	float CurHP;

	UPROPERTY(Replicated)
	float MaxHP;

	UPROPERTY(ReplicatedUsing = OnRep_CurMP)
	float CurMP;

	UPROPERTY(ReplicatedUsing = OnRep_AttackPower)
	float AttackPower;

	UPROPERTY(Replicated)
	float BaseAttackPower;

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_OnDeath();
		
};
