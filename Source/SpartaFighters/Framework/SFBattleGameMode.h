#pragma once

#include "CoreMinimal.h"
#include "Framework/SFGameModeBase.h"
#include "Framework/SFPlayerState.h"
#include "DataTable/CharacterDataRow.h"
#include "SFBattleGameMode.generated.h"

class ASFCharacterSpawner;
UCLASS()
class SPARTAFIGHTERS_API ASFBattleGameMode : public ASFGameModeBase
{
	GENERATED_BODY()
	
public:
	ASFBattleGameMode();

	virtual void BeginPlay() override;
    void HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer) override;

    void PollCharacterSpawnRequests();

    void RequestRespawn(AController* DeadController);
    void HandleCharacterSpawnRequest(APlayerController* PC);

protected:
    // Spawn Selected Character
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Data")
    UDataTable* CharacterDataTable;
    UPROPERTY()
    TArray<ASFCharacterSpawner*> SpawnPoints;

    AActor* GetAvailableSpawnPoint(bool bForRespawn = false) const;

    // Manage Rule
    UPROPERTY(EditDefaultsOnly, Category = "Battle")
    float BattleStartDelay = 3.0f;
    UPROPERTY(EditDefaultsOnly, Category = "Battle")
    float BattleTime = 180.0f;
    
    UFUNCTION()
    void StartBattle();
    
    UFUNCTION()
    void EndBattle();
    
    void HandlePlayerDeath(AController* DeadController);
    
    ASFPlayerState* CalculateWinner();
    
    void ReturnToLobby();

    // RPCs
    UFUNCTION(NetMulticast, Reliable)
    void Multicast_StartBattle();
    UFUNCTION(Client, Reliable)
    void Client_TravelToLobby(APlayerController* PC);

    // FTimerHandle 
    FTimerHandle BattleStartTimerHandle;
    FTimerHandle BattleTimerHandle;
    FTimerHandle ReturnLobbyTimerHandle;

    // SFBattleGameMode.h
    FTimerHandle SpawnPollingTimerHandle;
    TMap<APlayerController*, bool> bWaitingForSpawn;

};
