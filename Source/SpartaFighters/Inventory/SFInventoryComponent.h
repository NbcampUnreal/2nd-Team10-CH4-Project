

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Items/SFItemBase.h"
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

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void UpdateData();
	/*UFUNCTION(BlueprintCallable)
	void UseConsumable();*/
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool AddItem(const FName& ItemName, const FSoftObjectPath& ItemIconPath, FText ItemDescription, EItemType ItemType, int32 Quantity);
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool RemoveItem(FName ItemNameToRemove, int32 QuantityToRemove);
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Inventory")
	TArray<USFItemBase*> GetInventory() const { return Inventory; }
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Inventory")
	USFItemBase* FindItemByName(FName ItemNameToFind) const;
};
