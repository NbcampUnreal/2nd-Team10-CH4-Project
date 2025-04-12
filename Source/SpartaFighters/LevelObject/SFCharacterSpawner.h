#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SFCharacterSpawner.generated.h"

UCLASS()
class SPARTAFIGHTERS_API ASFCharacterSpawner : public AActor
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 SpawnIndex = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsRespawnPoint = false;

	FVector GetSpawnLocation() const { return GetActorLocation(); }
};
