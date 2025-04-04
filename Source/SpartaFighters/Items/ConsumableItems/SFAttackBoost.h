

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
	UFUNCTION()
	void RevertAttackBoost(ASFCharacter* InPlayerCharacter, float Amount);



	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Boost")
	float EffectDuration;

	virtual void ApplyConsumableEffect_Implementation(ASFCharacter* InPlayerCharacter) override;

	USFAttackBoost();
	
};
