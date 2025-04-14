#pragma once

#include "CoreMinimal.h"
#include "Framework/SFGameModeBase.h"
#include "SFCooperativeGameMode.generated.h"

class ASFCharacterSpawner;
class ASFPlayerState;

UCLASS()
class SPARTAFIGHTERS_API ASFCooperativeGameMode : public ASFGameModeBase
{
	GENERATED_BODY()
	
public:
	ASFCooperativeGameMode();

	virtual void BeginPlay() override;

	void PollCharacterSpawnRequests();
	void HandleCharacterSpawnRequest(APlayerController* PC);
	void StartBattle();
	void EndBattle();
	void ReturnToLobby();
	void Client_TravelToLobby_Implementation(APlayerController* PC);
	void HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer) override;

	ASFPlayerState* CalculateWinner();


	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Data")
	UDataTable* CharacterDataTable;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Battle")
	float BattleStartDelay;
	UPROPERTY(EditDefaultsOnly, Category = "Battle")
	float BattleTime;

	UPROPERTY()
	TArray<ASFCharacterSpawner*> SpawnPoints;

	TMap<APlayerController*, bool> bWaitingForSpawn;

	FTimerHandle SpawnPollingTimerHandle;
	FTimerHandle BattleStartTimerHandle;
	FTimerHandle BattleTimerHandle;
	FTimerHandle ReturnLobbyTimerHandle;

};
