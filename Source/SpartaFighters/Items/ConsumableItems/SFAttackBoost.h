#pragma once

#include "CoreMinimal.h"
#include "Items/ConsumableItems/SFConsumableBase.h"
#include "SFAttackBoost.generated.h"

/**
 * 
 */
UCLASS()
class SPARTAFIGHTERS_API USFAttackBoost : public USFConsumableBase
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Boost")
	float AttackBoostAmount;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Boost")
	float EffectDuration;

	UFUNCTION()
	void RevertAttackBoost(ASFCharacter* InPlayerCharacter, float Amount);

	virtual void Server_ApplyConsumableEffect_Implementation(ASFCharacter* InPlayerCharacter) override;

	USFAttackBoost();

private:
	FTimerHandle AttackBoostTimerHandle;
	
};
