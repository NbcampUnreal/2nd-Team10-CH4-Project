

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

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//Inventoryfunction
	//Server
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void Server_AddItemByClass(TSubclassOf<USFItemBase> ItemClass);
	//Server
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void Server_RemoveItem(FName ItemNameToRemove);
	//Client
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Inventory")
	TArray<USFItemBase*> GetInventory() const { return Inventory; }
	//Client
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Inventory")
	USFItemBase* FindItemByName(FName ItemNameToFind) const;

	//Equipment function
	//Server
	UFUNCTION(BlueprintCallable, Category = "Equipment")
	void Server_EquipItem(FName ItemNameToEquip, SFEquipSlot EquipSlot);
	//Server
	UFUNCTION(BlueprintCallable, Category = "Equipment")
	void Server_UnequipItem(SFEquipSlot EquipSlot);
	//Client
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Equipment")
	USFItemBase* GetEquippedItem(SFEquipSlot EquipSlot) const;
	//Client
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Equipment")
	bool IsItemEquipped(FName ItemName) const;

	//Multiplay related props
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated, Category = "Inventory", meta = (AllowPrivateAccess = "true"))
	TArray<USFItemBase*> Inventory;
	//TODO:get rid of TMAP(unable to replicate)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Equipment", meta = (AllowPrivateAccess = "true"))
	TMap<SFEquipSlot, USFItemBase*> EquippedItems;

	//Tell client when inventory is updated
	UPROPERTY(ReplicatedUsing = OnRep_InventoryUpdated)
	bool bInventoryUpdated;
	UFUNCTION()
	virtual void OnRep_InventoryUpdated();

	//Tell client when Equipment is updated
	UPROPERTY(ReplicatedUsing = OnRep_EquippedItemsUpdated)
	bool bEquippedItemsUpdated;
	UFUNCTION()
	virtual void OnRep_EquippedItemsUpdated();

	//Update data locally in server
	void Internal_UpdateData();


	

};
