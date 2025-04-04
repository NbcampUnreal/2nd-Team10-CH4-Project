

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Items/SFItemBase.h"
#include "SFItemPickup.generated.h"

UCLASS()
class SPARTAFIGHTERS_API ASFItemPickup : public AActor
{
	GENERATED_BODY()
	
public:	
	ASFItemPickup();
	virtual void BeginPlay() override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Pickup", meta = (AllowPrivateAccess = "true"))
	class USphereComponent* OverlapSphere;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Pickup")
	TSubclassOf<USFItemBase> ItemClass;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Pickup")
	class USFItemBase* PickupItemInstance;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);



};
