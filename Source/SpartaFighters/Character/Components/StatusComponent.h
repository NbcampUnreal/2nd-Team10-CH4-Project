

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
	// �ʱ�ȭ
	void InitializeStatus();

	// �� ���
	float GetStatusValue(EStatusType Type) const;

	// �� ����
	void ModifyStatus(EStatusType Type, float Amount);

	// ü�� ����
	void ModifyHP(float Amount);
	UFUNCTION()
	void OnRep_CurHP();

	// ���� ���� ��ü ��ȯ
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
