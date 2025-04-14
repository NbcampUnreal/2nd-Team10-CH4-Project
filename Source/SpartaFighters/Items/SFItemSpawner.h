#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Items/SFItemPickup.h"
#include "SFItemSpawner.generated.h"

class UBoxComponent;
class USceneComponent;
class ASFItemPickup;
class ASFItemSpawnPoint;

UCLASS()
class SPARTAFIGHTERS_API ASFItemSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	ASFItemSpawner();

protected:
	virtual void BeginPlay() override;

	UPROPERTY()
	TArray<ASFItemSpawnPoint*> SpawnPoints;

	UPROPERTY(EditAnywhere, Category = "Spawn")
	TArray<TSubclassOf<class ASFItemPickup>> ItemClasses;

	UPROPERTY(EditAnywhere, Category = "Spawn")
	float RespawnTime = 3.0f;

	TMap<ASFItemSpawnPoint*, ASFItemPickup*> SpawnedItems;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<UBoxComponent> SpawnArea;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<USceneComponent> SceneRoot;

	UFUNCTION(BlueprintCallable, Category = "Spawn")
	void SpawnItems();
	UFUNCTION(BlueprintCallable, Category = "Spawn")
	void RespawnItem(ASFItemSpawnPoint* SpawnPoint);
	UFUNCTION(BlueprintCallable, Category = "Spawn")
	void OnItemDestroyed(AActor* DestroyedActor);

};
