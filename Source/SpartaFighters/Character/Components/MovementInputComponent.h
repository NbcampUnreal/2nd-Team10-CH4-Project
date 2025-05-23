#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MovementInputComponent.generated.h"

class UEnhancedInputComponent;
class ASFPlayerController;
class ASFCharacter;
struct FInputActionValue;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SPARTAFIGHTERS_API UMovementInputComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UMovementInputComponent();

	UPROPERTY()
	ASFCharacter* OwnerCharacter;

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void SetupInput(UEnhancedInputComponent* Input, ASFPlayerController* PlayerController, ASFCharacter* SFCharacter);
	void Move(const FInputActionValue& value);
	void StartJump(const FInputActionValue& Value);
	void StopJump(const FInputActionValue& Value);
	void ResetJumpCount();

	void StartRoll(const FInputActionValue& Value);
	void EndRoll(const FInputActionValue& Value);

private:
	int32 JumpCount;
};
