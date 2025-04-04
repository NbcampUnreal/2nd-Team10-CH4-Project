

#pragma once

#include "CoreMinimal.h"
#include "Items/EquipItems/ExclusiveItems/SFExclusiveBase.h"
#include "SFSword.generated.h"

/**
 * 
 */
UCLASS()
class SPARTAFIGHTERS_API USFSword : public USFExclusiveBase
{
	GENERATED_BODY()
	
public:
	USFSword();
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Sword")
	float AttackDamage;


};
