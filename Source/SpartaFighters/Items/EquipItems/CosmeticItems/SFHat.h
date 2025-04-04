

#pragma once

#include "CoreMinimal.h"
#include "Items/EquipItems/CosmeticItems/SFCosmeticBase.h"
#include "SFHat.generated.h"

/**
 * 
 */
UCLASS()
class SPARTAFIGHTERS_API USFHat : public USFCosmeticBase
{
	GENERATED_BODY()
	
public:
	USFHat();

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Hat")
	TSoftObjectPtr<UStaticMesh> HatMesh;

	
	virtual void ApplyCosmetic_Implementation(AActor* Owner) override;
	virtual void RemoveCosmetic_Implementation(AActor* Owner) override;

	UFUNCTION(BlueprintCallable)
	void SetHatMesh(TSoftObjectPtr<UStaticMesh>Mesh);
};
