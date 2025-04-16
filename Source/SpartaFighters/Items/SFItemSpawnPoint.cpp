#include "Items/SFItemSpawnPoint.h"


ASFItemSpawnPoint::ASFItemSpawnPoint()
{
 	PrimaryActorTick.bCanEverTick = false;
}

void ASFItemSpawnPoint::BeginPlay()
{
	Super::BeginPlay();
	SpawnItem();
}

void ASFItemSpawnPoint::SpawnItem()
{
	TSubclassOf<ASFItemPickup> ItemToSpawn = nullptr;

	if (ItemToSpawn)
	{
		FActorSpawnParameters SpawnParams;
		GetWorld()->SpawnActor<AActor>(ItemToSpawn, GetActorLocation() + SpawnOffset, GetActorRotation(), SpawnParams);

		UE_LOG(LogTemp, Log, TEXT("%s is successfully spawned"), *ItemToSpawn->GetName());
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("No item to spawn."));
	}

}

