

#pragma once

#include "CoreMinimal.h"
#include "Character/SFCharacter.h"
#include "AI/AICharacterController.h"
#include "SFEnemyCharacter.generated.h"

UCLASS()
class SPARTAFIGHTERS_API ASFEnemyCharacter : public ASFCharacter
{
	GENERATED_BODY()
	

public:
	virtual void BeginPlay() override;

	void ChaseTarget();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	AAICharacterController* AIController;

};
