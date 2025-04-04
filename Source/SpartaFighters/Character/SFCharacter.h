#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Character/Stats/StatusContainerInterface.h"
#include "DataTable/SkillDataRow.h"
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

public:
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UDataTable* SkillDataTable;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
	//FSkillDataRow SkillDataRow;

protected:
	// TO DO : Seperate Componenets
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	ASFPlayerController* PlayerController;

	// Movement
	void Move(const FInputActionValue& Value);

	void StartJump(const FInputActionValue& Value);
	void StopJump(const FInputActionValue& Value);

	void StartRoll(const FInputActionValue& Value);
	void StopRoll(const FInputActionValue& Value);

	void CrouchPressed(const FInputActionValue& Value);
	void CrouchReleased(const FInputActionValue& Value);

	// Ability
	void AttackPressed(const FInputActionValue& Value);
	void AttackReleased(const FInputActionValue& Value);

	void SkillAttackPressed(const FInputActionValue& Value);
	void SkillAttackReleased(const FInputActionValue& Value);

	void GuardPressed(const FInputActionValue& Value);
	void GuardReleased(const FInputActionValue& Value);

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


private:
	bool bIsInAir;
	bool bIsRoll;
	bool bIsCrouch;
	bool bIsAttack;
	bool bIsGuard;
	
};
