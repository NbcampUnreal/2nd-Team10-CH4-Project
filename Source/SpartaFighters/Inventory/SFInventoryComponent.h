

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

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void UpdateData();
	UFUNCTION(BlueprintCallable)
	void UseConsumable();
	UFUNCTION(BlueprintCallable)
	void AddItem(USFItemBase* Item);
	UFUNCTION(BlueprintCallable)
	void RemoveItem(USFItemBase*Item);
};
