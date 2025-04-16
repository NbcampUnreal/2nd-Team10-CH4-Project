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
    float BattleStartDelay;
    UPROPERTY(EditDefaultsOnly, Category = "Battle")
    float BattleTime;

    float ReturnToLobbyTime;
    
    UFUNCTION()
    void StartBattle();
    
    UFUNCTION()
    void EndBattle();
    
    void HandlePlayerDeath(AController* DeadController);
    
    ASFPlayerState* CalculateWinner();
    
    void ReturnToLobby();

    void ServerTravelToLobby();

    // RPCs
    UFUNCTION(NetMulticast, Reliable)
    void Multicast_StartBattle();

    // FTimerHandle 
    FTimerHandle BattleStartTimerHandle;
    FTimerHandle BattleTimerHandle;
    FTimerHandle ReturnLobbyTimerHandle;
    FTimerHandle LazyServerTravelTimerHandle;

    // SFBattleGameMode.h
    FTimerHandle SpawnPollingTimerHandle;
    TMap<APlayerController*, bool> bWaitingForSpawn;








    // AI 관련 설정 추가
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AI")
    TSubclassOf<ACharacter> AICharacterClass; // 스폰할 AI 캐릭터 클래스

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AI")
    int32 NumAICharactersToSpawn = 2; // 스폰할 AI 캐릭터 수


    // AI 스폰 함수 추가
    void SpawnAICharacters();
};
