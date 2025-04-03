

#pragma once

#include "CoreMinimal.h"
#include "Items/EquipItems/SFEquipableBase.h"
#include "SFExclusiveBase.generated.h"

/**
 * 
 */
UCLASS()
class SPARTAFIGHTERS_API USFExclusiveBase : public USFEquipableBase
{
	GENERATED_BODY()
public:	
	virtual void EquipItem()override;
	
};
