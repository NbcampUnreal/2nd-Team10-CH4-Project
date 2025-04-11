#pragma once

#include "CoreMinimal.h"
#include "Items/EquipItems/CommonItems/SFCommonBase.h"
#include "SFNecklace.generated.h"

/**
 * 
 */
UCLASS()
class SPARTAFIGHTERS_API USFNecklace : public USFCommonBase
{
	GENERATED_BODY()
	
public:
	USFNecklace();
	//Necklace stat property
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Necklace")
	float AdditionalAttackPower;

};
