#pragma once

#include "CoreMinimal.h"
#include "Items/ConsumableItems/SFConsumableBase.h"
#include "SFGold.generated.h"

UCLASS()
class SPARTAFIGHTERS_API USFGold : public USFConsumableBase
{
	GENERATED_BODY()
	
public:
	USFGold();

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Gold")
	float ObtainedGold;

	virtual void Server_ApplyConsumableEffect_Implementation(ASFCharacter* InPlayerCharacter) override;
};
