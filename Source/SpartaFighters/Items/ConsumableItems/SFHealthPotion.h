#pragma once

#include "CoreMinimal.h"
#include "Items/ConsumableItems/SFConsumableBase.h"
#include "SFHealthPotion.generated.h"

UCLASS()
class SPARTAFIGHTERS_API USFHealthPotion : public USFConsumableBase
{
	GENERATED_BODY()

public:
	USFHealthPotion();
	
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Potion")
	float RecoveryAmount;

	
	virtual void Server_ApplyConsumableEffect_Implementation(ASFCharacter* InPlayerCharacter) override;

	
};
