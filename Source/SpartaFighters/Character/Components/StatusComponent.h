

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Character/Stats/FStatusStruct.h"
#include "StatusComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDeathEvent);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHealthChanged, AActor*, OwnerActor, float, NewHP);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SPARTAFIGHTERS_API UStatusComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UStatusComponent();

protected:
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	// 초기화
	void InitializeStatus();

	// 값 얻기
	float GetStatusValue(EStatusType Type) const;

	// 값 변경
	void ModifyStatus(EStatusType Type, float Amount);

	// 체력 관련
	void ModifyHP(float Amount);
	UFUNCTION()
	void OnRep_CurHP();

	// 상태 구조 전체 반환
	FStatusStruct GetStruct() const { return StatusStruct; }

	UPROPERTY(BlueprintAssignable)
	FOnHealthChanged OnHealthChanged;

	UPROPERTY(BlueprintAssignable)
	FOnDeathEvent OnDeath;

private:
	UPROPERTY(EditAnywhere, Category = "Status")
	FStatusStruct StatusStruct;

	UPROPERTY(ReplicatedUsing = OnRep_CurHP)
	float CurHP;

	UPROPERTY(Replicated)
	float MaxHP;

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_OnDeath();
	void Die();

		
};
