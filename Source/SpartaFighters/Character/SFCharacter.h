#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Character/Stats/StatusContainerInterface.h"
#include "SFCharacter.generated.h"

class UMovementInputComponent;
class UStatusContainerComponent;

UCLASS()
class SPARTAFIGHTERS_API ASFCharacter : public ACharacter, public IStatusContainerInterface
{
	GENERATED_BODY()

public:
	ASFCharacter();

	virtual UStatusContainerComponent* GetStatusContainerComponent() const override;

protected:
	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void InitializeCharacterProperties();
	void Landed(const FHitResult& Hit);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MovementInput")
	UMovementInputComponent* MovementInputComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stat")
	UStatusContainerComponent* StatusContainerComponent;

public:	
	virtual void Tick(float DeltaTime) override;
	
};
