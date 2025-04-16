#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Items/SFItemPickup.h"
#include "SFItemSpawnPoint.generated.h"

UCLASS()
class SPARTAFIGHTERS_API ASFItemSpawnPoint : public AActor
{
	GENERATED_BODY()
	
public:	
	ASFItemSpawnPoint();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void SpawnItem();

public:
	UPROPERTY(EditAnywhere, Category = "Spawn")
	TArray<TSubclassOf<ASFItemPickup>> ConsumableItemClasses;

	UPROPERTY(EditAnywhere, Category = "Spawn")
	FVector SpawnOffset = FVector::ZeroVector;
};
