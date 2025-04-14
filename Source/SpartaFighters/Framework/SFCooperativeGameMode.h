#pragma once

#include "CoreMinimal.h"
#include "Framework/SFGameModeBase.h"
#include "SFCooperativeGameMode.generated.h"

class ASFCharacterSpawner;

UCLASS()
class SPARTAFIGHTERS_API ASFCooperativeGameMode : public ASFGameModeBase
{
	GENERATED_BODY()
	
public:
	ASFCooperativeGameMode();

	virtual void BeginPlay() override;

	void PollCharacterSpawnRequests();
	void HandleCharacterSpawnRequest(APlayerController* PC);
	void HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer) override;
	void RequestRespawn(AController* DeadController);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Data")
	UDataTable* CharacterDataTable;

protected:
	UPROPERTY()
	TArray<ASFCharacterSpawner*> SpawnPoints;

	TMap<APlayerController*, bool> bWaitingForSpawn;
	FTimerHandle SpawnPollingTimerHandle;
};
