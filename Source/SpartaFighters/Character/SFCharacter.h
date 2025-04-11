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
struct FInputActionValue;
struct FSkillDataRow;

UCLASS()
class SPARTAFIGHTERS_API ASFCharacter : public ACharacter, public IStatusContainerInterface
{
	GENERATED_BODY()

public:
	ASFCharacter();

	virtual UStatusContainerComponent* GetStatusContainerComponent() const override;

protected:
	virtual void BeginPlay() override;

	UPROPERTY()
	TArray<TObjectPtr<UObject>> AttackHandlers;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UDataTable* SkillDataTable;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
	FSkillDataRow* CurrentSkillDataBuffer;

	void PerformAttack(int32 AttackIndex);
	void AddAttackHandler(UObject* AttackHandler);

	virtual void AttackTrace();

	//UPROPERTY(EditAnywhere, Category = "Combat")
	//FName RightHandSocketName = "RightHandSocket"; // 소켓 이름 (애니메이션 소켓 기준)

	//UPROPERTY(EditAnywhere, Category = "Combat")
	//float JabTraceLength = 100.f;

	//UPROPERTY(EditAnywhere, Category = "Combat")
	//float JabTraceRadius = 15.f;

protected:
	// TO DO : Seperate Componenets
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	ASFPlayerController* PlayerController;

	// Movement
	void Move(const FInputActionValue& Value);

	void StartJump();
	void StopJump();

	void SpecialMovePressed(const FInputActionValue& Value);
	void SpecialMoveReleased();

	void CrouchPressed();
	void CrouchReleased();

	// Ability
	void AttackPressed();
	void AttackReleased();

	void SkillAttackPressed();
	void SkillAttackReleased();

	void GuardPressed();
	void GuardReleased();

	virtual float TakeDamage(
		float DamageAmount,
		struct FDamageEvent const& DamageEvent,
		AController* EventInstigator,
		AActor* DamageCauser) override;


//protected:
//	void InitializeCharacterProperties();
//	void Landed(const FHitResult& Hit);
//
//	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MovementInput")
//	UMovementInputComponent* MovementInputComponent;
//
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stat")
	UStatusContainerComponent* StatusContainerComponent;

	// TO DO : Low Jump Function
	UPROPERTY(EditDefaultsOnly, Category = "Jump")
	float MaxJumpHoldTime = 0.3f; // 최대 유지 시간

	FTimerHandle JumpHoldTimer;

	int32 NomalAttackComboCount = 3;
	int32 PowerAttackComboCount = 2;

private:
	bool bIsInAir;
	bool bIsRoll;
	bool bIsCrouch;
	bool bIsAttack;
	bool bIsGuard;
	bool bIsSpecialMove;

};
