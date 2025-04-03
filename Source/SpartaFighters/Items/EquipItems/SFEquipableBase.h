

#pragma once

#include "CoreMinimal.h"
#include "Items/SFItemBase.h"
#include "SFEquipableBase.generated.h"

/**
 * 
 */
UCLASS()
class SPARTAFIGHTERS_API USFEquipableBase : public USFItemBase
{
	GENERATED_BODY()
	
public:
	virtual void EquipItem();
};
