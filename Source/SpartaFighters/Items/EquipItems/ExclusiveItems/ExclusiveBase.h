

#pragma once

#include "CoreMinimal.h"
#include "Items/EquipItems/EquipableBase.h"
#include "ExclusiveBase.generated.h"

/**
 * 
 */
UCLASS()
class SPARTAFIGHTERS_API UExclusiveBase : public UEquipableBase
{
	GENERATED_BODY()
public:	
	virtual void EquipItem()override;
	
};
