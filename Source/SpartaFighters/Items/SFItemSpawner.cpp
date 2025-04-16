#include "Items/SFItemSpawner.h"
#include "Items/SFItemSpawnPoint.h"
#include "Components/BoxComponent.h"
#include "Components/SceneComponent.h"
#include "Kismet/GameplayStatics.h"

ASFItemSpawner::ASFItemSpawner()
{
	PrimaryActorTick.bCanEverTick = false;
	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	SetRootComponent(SceneRoot);
	SpawnArea = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawnArea"));
	SpawnArea->SetupAttachment(SceneRoot);
}

void ASFItemSpawner::BeginPlay()
{
	Super::BeginPlay();
	
	if (HasAuthority())
	{
		TArray<AActor*> FoundActors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASFItemSpawnPoint::StaticClass(), FoundActors);

		for (AActor* Actor : FoundActors)
		{
			ASFItemSpawnPoint* SpawnPoint = Cast<ASFItemSpawnPoint>(Actor);
			if (SpawnPoint)
			{
				SpawnPoints.Add(SpawnPoint);
			}
		}
		SpawnItems();
	}
}

void ASFItemSpawner::SpawnItems()
{
	if (ItemClasses.Num() == 0 || SpawnPoints.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("[ItemSpawner] class is none "));
		return;
	}

	for (ASFItemSpawnPoint* SpawnPoint : SpawnPoints)
	{
		if (SpawnedItems.Contains(SpawnPoint) && SpawnedItems[SpawnPoint] != nullptr)
		{
			continue;
		}
		int32 RandomIndex = FMath::RandRange(0, ItemClasses.Num() - 1);
		TSubclassOf<ASFItemPickup> RandomItemClass = ItemClasses[RandomIndex];

		if (RandomItemClass)
		{
			ASFItemPickup* SpawnedItem = GetWorld()->SpawnActor<ASFItemPickup>(RandomItemClass, SpawnPoint->GetActorLocation(), FRotator::ZeroRotator);
			if (SpawnedItem)
			{
				UE_LOG(LogTemp, Log, TEXT("[ItemSpawner] is spawned: %s (Location: %s)"), *SpawnedItem->GetName(), *SpawnPoint->GetName());

				SpawnedItem->OnDestroyed.AddDynamic(this, &ASFItemSpawner::OnItemDestroyed);

				SpawnedItems.Add(SpawnPoint, SpawnedItem);
			}
		}
	}
}

void ASFItemSpawner::OnItemDestroyed(AActor* DestroyedActor)
{
	if (!DestroyedActor) return;

	ASFItemPickup* DestroyedItem = Cast<ASFItemPickup>(DestroyedActor);
	if (!DestroyedItem) return;

	UE_LOG(LogTemp, Warning, TEXT("[ItemSpawner] is deleted: %s"), *DestroyedItem->GetName());

	for (auto& Elem : SpawnedItems)
	{
		if (Elem.Value == DestroyedItem)
		{
			ASFItemSpawnPoint* SpawnPoint = Elem.Key;
			SpawnedItems[SpawnPoint] = nullptr; 

			UE_LOG(LogTemp, Log, TEXT("[ItemSpawner] %.1f to respawn item in [%s]"), RespawnTime, *SpawnPoint->GetName());

			FTimerHandle RespawnTimerHandle;
			GetWorld()->GetTimerManager().SetTimer(RespawnTimerHandle, FTimerDelegate::CreateLambda([this, SpawnPoint]()
				{
					if (SpawnPoint)
					{
						UE_LOG(LogTemp, Log, TEXT("[ItemSpawner] Respawn"));
						this->SpawnItems();
					}
				}), RespawnTime, false);

			break;
		}
	}
}

void ASFItemSpawner::RespawnItem(ASFItemSpawnPoint* SpawnPoint)
{
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(
		TimerHandle,
		FTimerDelegate::CreateUObject(this, &ASFItemSpawner::SpawnItems),
		RespawnTime,
		false
	);
}
