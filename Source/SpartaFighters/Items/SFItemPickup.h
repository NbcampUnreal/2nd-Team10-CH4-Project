#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Items/ConsumableItems/SFConsumableBase.h"
#include "Net/UnrealNetwork.h"
#include "SFItemPickup.generated.h"

class USphereComponent;

UCLASS()
class SPARTAFIGHTERS_API ASFItemPickup : public AActor
{
	GENERATED_BODY()
	
public:	
	ASFItemPickup();
	virtual void BeginPlay() override;
	UFUNCTION(BlueprintCallable, Category = "Pickup|Visual")
	void SetPickupMesh(UStaticMesh* NewMesh);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated, Category = "Pickup", meta = (AllowPrivateAccess = "true"))
	class USphereComponent* OverlapSphere;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated, Category = "Pickup", meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* PickupMesh;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Pickup")
	TSubclassOf<USFConsumableBase> ItemClass;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated, Category = "Pickup")
	class USFItemBase* PickupItemInstance;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Pickup")
	void Server_UsePickup(ASFCharacter* PlayerCharacter);
	virtual void Server_UsePickup_Implementation(ASFCharacter* PlayerCharacter);

};
