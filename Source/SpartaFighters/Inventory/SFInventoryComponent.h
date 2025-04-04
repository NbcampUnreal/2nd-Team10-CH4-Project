

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Items/SFItemBase.h"
#include "Items/EquipItems/SFEquipableBase.h"
#include "SFInventoryComponent.generated.h"




UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SPARTAFIGHTERS_API USFInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USFInventoryComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory")
	TArray<USFItemBase*> Inventory;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Equipment")
	TMap<SFEquipSlot, USFItemBase*> EquippedItems;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void UpdateData();

	//Inventoryfunction
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool AddItemByClass(TSubclassOf<USFItemBase> ItemClass);
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool RemoveItem(FName ItemNameToRemove);
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Inventory")
	TArray<USFItemBase*> GetInventory() const { return Inventory; }
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Inventory")
	USFItemBase* FindItemByName(FName ItemNameToFind) const;

	//Equipment function
	UFUNCTION(BlueprintCallable, Category = "Equipment")
	bool EquipItem(FName ItemNameToEquip, SFEquipSlot EquipSlot);
	UFUNCTION(BlueprintCallable, Category = "Equipment")
	bool UnequipItem(SFEquipSlot EquipSlot);
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Equipment")
	USFItemBase* GetEquippedItem(SFEquipSlot EquipSlot) const;
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Equipment")
	bool IsItemEquipped(FName ItemName) const;
};
