

#pragma once

#include "CoreMinimal.h"
#include "Items/ItemBase.h"
#include "Character/SFCharacter.h"
#include "ConsumableBase.generated.h"

/**
 * 
 */
UCLASS()
class SPARTAFIGHTERS_API UConsumableBase : public UItemBase
{
	GENERATED_BODY()
	
public:
	void ApplyConsumableEffect(ASFCharacter* InPlayerCharacter);
};
