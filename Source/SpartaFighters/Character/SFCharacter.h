#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SFCharacter.generated.h"

class UMovementInputComponent;

UCLASS()
class SPARTAFIGHTERS_API ASFCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ASFCharacter();

protected:
	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Input")
	UMovementInputComponent* MovementInputComponent;

public:	
	virtual void Tick(float DeltaTime) override;
	
};
