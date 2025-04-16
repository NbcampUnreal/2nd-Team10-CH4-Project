

#pragma once

#include "CoreMinimal.h"
#include "Items/EquipItems/SFEquipableBase.h"
#include "SFExclusiveBase.generated.h"

UENUM(BlueprintType)
enum class ESFRequiredClass : uint8
{
	None UMETA(DisplayName = "None"),
	Warrior UMETA(DisplayName = "Warrior"),
	Mage UMETA(DisplayName = "Mage"),
	Archer UMETA(DisplayName = "Archer")
};

UCLASS()
class SPARTAFIGHTERS_API USFExclusiveBase : public USFEquipableBase
{
	GENERATED_BODY()
public:	
	USFExclusiveBase();

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Exclusive")
	ESFRequiredClass RequiredClass;

	//skill on and off
	virtual void OnEquipped_Implementation(AActor* Owner) override;
	virtual void OnUnequipped_Implementation(AActor* Owner) override;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Exclusive")
	bool CanEquip(AActor* Owner);
	
};
