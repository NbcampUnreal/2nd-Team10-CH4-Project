
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DataTable/SkillDataRow.h"
#include "Character/Components/StateComponent.h"
#include "SkillComponent.generated.h"

UENUM(BlueprintType)
enum class ESkillType : uint8
{
	BasicAttack,
	PowerAttack,
	Skill,
	Guard,
};


class ASFCharacter;
class UAnimInstance;
class UAnimMontage;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SPARTAFIGHTERS_API USkillComponent : public UActorComponent
{
	GENERATED_BODY()
	
public:
	USkillComponent();

	void Initialize(UDataTable* InSkillDataTable, UStateComponent* InStateComp, ASFCharacter* Character);

	// Handle Basic Attack
	void HandleInputBasicAttack();

	UFUNCTION(Server, Reliable)
	void Server_HandleBasicAttack();

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_HandleBasicAttack(FSkillDataRow Data);

	void HandleBasicAttack(FSkillDataRow* Data);


	// Handle Skill Attack
	void HandleInputSkillAttack();

	UFUNCTION(Server, Reliable)
	void Server_HandleSkillAttack();

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_HandleSkillAttack(FSkillDataRow Data);

	void HandleSkillAttack(FSkillDataRow* Data);



	// Handle Dodge Attack
	void HandleInputDodge();

	UFUNCTION(Server, Reliable)
	void Server_HandleDodge();

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_HandleDodge(ECharacterState State);

	void HandleDodge(ECharacterState CurrentState);


	// Handle Animation
	void PlayAnimMontage();
	void OnMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	void PlayAnimMontage(FSkillDataRow* Data);
	void OnMontageEnded();

	void PerformAttackTrace();
	void ClearHitActors();

	void CanNextCombo();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	int MaxComboCount = 3;
	int ComboCount = 0;

private:
	UPROPERTY()
	UDataTable* SkillDataTable;

	UPROPERTY()
	UStateComponent* StateComponent;

	UPROPERTY()
	ASFCharacter* OwnerCharacter;

	FSkillDataRow* CurrentSkillData;

	TSet<AActor*> AlreadyHitActors;
};
