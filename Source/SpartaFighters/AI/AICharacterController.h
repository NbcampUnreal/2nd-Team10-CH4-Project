

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "AICharacterController.generated.h"

/**
 * 
 */
UCLASS()
class SPARTAFIGHTERS_API AAICharacterController : public AAIController
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	UBehaviorTree* BehaviorTree;
};
