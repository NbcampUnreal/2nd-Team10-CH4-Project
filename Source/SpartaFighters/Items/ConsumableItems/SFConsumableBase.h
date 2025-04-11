#pragma once

#include "CoreMinimal.h"
#include "Items/SFItemBase.h"
#include "Character/SFCharacter.h"
#include "SFConsumableBase.generated.h"

/**
 * 
 */
UCLASS()
class SPARTAFIGHTERS_API USFConsumableBase : public USFItemBase
{
	GENERATED_BODY()
	
public: 
	USFConsumableBase();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Consumable")
	void ApplyConsumableEffect(ASFCharacter* InPlayerCharacter);
	virtual void ApplyConsumableEffect_Implementation(ASFCharacter* InPlayerCharacter);



};
