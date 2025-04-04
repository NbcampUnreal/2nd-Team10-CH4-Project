

#pragma once

#include "CoreMinimal.h"
#include "Items/ConsumableItems/SFConsumableBase.h"
#include "SFHealthPotion.generated.h"

/**
 * 
 */
UCLASS()
class SPARTAFIGHTERS_API USFHealthPotion : public USFConsumableBase
{
	GENERATED_BODY()

public:
	USFHealthPotion();
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Potion")
	float RecoveryAmount;

	
	virtual void ApplyConsumableEffect_Implementation(ASFCharacter* InPlayerCharacter) override;

	
};
