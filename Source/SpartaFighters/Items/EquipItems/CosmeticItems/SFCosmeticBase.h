#pragma once

#include "CoreMinimal.h"
#include "Items/EquipItems/SFEquipableBase.h"
#include "SFCosmeticBase.generated.h"

/**
 * 
 */
UCLASS()
class SPARTAFIGHTERS_API USFCosmeticBase : public USFEquipableBase
{
	GENERATED_BODY()
	

public:
	USFCosmeticBase();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Cosmetic")
	void ApplyCosmetic(AActor* Owner);
	virtual void ApplyCosmetic_Implementation(AActor* Owner);


	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Cosmetic")
	void RemoveCosmetic(AActor* Owner);
	virtual void RemoveCosmetic_Implementation(AActor* Owner);
};
