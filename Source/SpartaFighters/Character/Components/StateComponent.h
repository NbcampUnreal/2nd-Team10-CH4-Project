

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StateComponent.generated.h"

UENUM(BlueprintType)
enum class ECharacterState : uint8
{
	Idle,
	Moving,
	Attacking,
	Guarding,
	Crouching,
	Rolling,
	InAir
};

UENUM(BlueprintType)
enum class ECharacterSpecialState : uint8
{
	None,
	Invincible,
	Stunned,
};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SPARTAFIGHTERS_API UStateComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UStateComponent();

	UFUNCTION(BlueprintCallable)
	void UpdateState(ACharacter* Character);

	void SetState(ECharacterState NewState);

	ECharacterState GetState() const { return CurrentState; }
	bool CanMove() const { return !bIsInAction && CurrentState != ECharacterState::Crouching; };
	bool CanJump() const { return !bIsInAction; };
	bool CanCrouch() const { return !bIsInAction; };

	void SetIsInAction(bool bIn);
	bool IsInAction() const;
	bool IsInState(ECharacterState CheckState) const;

	ECharacterSpecialState GetSpecailState() const { return CurrentSpecialState; }
	void SetSpecialState(ECharacterSpecialState NewState);
	void ResetSpecialState();

protected:
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION()
	void OnRep_CurrentState();

	UFUNCTION()
	void OnRep_SpecialState();

private:
	UPROPERTY(ReplicatedUsing = OnRep_CurrentState)
	ECharacterState CurrentState;

	UPROPERTY(Replicated)
	bool bIsInAction;

	UPROPERTY(ReplicatedUsing = OnRep_SpecialState)
	ECharacterSpecialState CurrentSpecialState;
};
