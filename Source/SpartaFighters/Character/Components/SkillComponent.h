
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


//struct FSkillDataRow;
//class ECharacterState;
class UAnimInstance;
class UAnimMontage;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SPARTAFIGHTERS_API USkillComponent : public UActorComponent
{
	GENERATED_BODY()
	
public:
	USkillComponent();

	void Initialize(UDataTable* InSkillDataTable, UStateComponent* InStateComp, ACharacter* Character);

	// Handle Basic Attack
	void HandleInputBasicAttack();

	UFUNCTION(Server, Reliable)
	void Server_HandleBasicAttack();

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_HandleBasicAttack(ECharacterState State);

	void HandleBasicAttack(ECharacterState CurrentState);


	// Handle Skill Attack
	void HandleInputSkillAttack();

	UFUNCTION(Server, Reliable)
	void Server_HandleSkillAttack();

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_HandleSkillAttack(ECharacterState State);

	void HandleSkillAttack(ECharacterState CurrentState);

	// Handle Skill Attack
	void HandleInputDodge();

	UFUNCTION(Server, Reliable)
	void Server_HandleDodge();

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_HandleDodge(ECharacterState State);

	void HandleDodge(ECharacterState CurrentState);


	// Handle Animation
	void PlayAnimMontage(const FName& RowName);
	void OnMontageEnded(UAnimMontage* Montage, bool bInterrupted);


	void PerformAttackTrace();
	void ClearHitActors();

private:
	UPROPERTY()
	UDataTable* SkillDataTable;

	UPROPERTY()
	UStateComponent* StateComponent;

	UPROPERTY()
	ACharacter* OwnerCharacter;

	FSkillDataRow* CurrentSkillData;

	TSet<AActor*> AlreadyHitActors;
};
