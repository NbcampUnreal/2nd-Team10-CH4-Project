

#pragma once

#include "CoreMinimal.h"
#include "Items/ItemBase.h"
#include "EquipableBase.generated.h"

/**
 * 
 */
UCLASS()
class SPARTAFIGHTERS_API UEquipableBase : public UItemBase
{
	GENERATED_BODY()
	
public:
	virtual void EquipItem();
};
