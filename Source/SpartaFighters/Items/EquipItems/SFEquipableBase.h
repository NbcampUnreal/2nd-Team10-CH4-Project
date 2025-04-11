#pragma once

#include "CoreMinimal.h"
#include "Items/SFItemBase.h"
#include "SFEquipableBase.generated.h"

UENUM(BlueprintType)
enum class SFEquipSlot : uint8
{
	CommonSlot UMETA(DisplayName = "Common"),
	ExclusiveSlot UMETA(DisplayName = "Exclusive"),
	CosmeticSlot UMETA(DisplayName = "Cosmetic"),
};


UCLASS()
class SPARTAFIGHTERS_API USFEquipableBase : public USFItemBase
{
	GENERATED_BODY()
	
public:
	USFEquipableBase();
	//EquipSlot info
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Equipment")
	SFEquipSlot EquipSlot;

	//Equip item
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Equipment")
	void OnEquipped(AActor* Owner);
	virtual void OnEquipped_Implementation(AActor* Owner);

	//Unequip item
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Equipment")
	void OnUnequipped(AActor* Owner);
	virtual void OnUnequipped_Implementation(AActor* Owner);
};
