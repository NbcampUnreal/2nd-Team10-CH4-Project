#pragma once

#include "CoreMinimal.h"
#include "Framework/SFGameModeBase.h"
#include "Framework/SFPlayerState.h"
#include "SFBattleGameMode.generated.h"

UCLASS()
class SPARTAFIGHTERS_API ASFBattleGameMode : public ASFGameModeBase
{
	GENERATED_BODY()
	
public:
	ASFBattleGameMode();

	virtual void BeginPlay() override;

protected:
    UPROPERTY(EditDefaultsOnly, Category = "Battle")
    float BattleStartDelay = 3.0f;
    UPROPERTY(EditDefaultsOnly, Category = "Battle")
    float BattleTime = 180.0f;
    UFUNCTION()
    void StartBattle();
    UFUNCTION()
    void EndBattle();
    
    void HandlePlayerDeath(AController* DeadController);
    virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;
    ASFPlayerState* CalculateWinner();
    void ReturnToLobby();

    UFUNCTION(NetMulticast, Reliable)
    void Multicast_StartBattle();
    UFUNCTION(Client, Reliable)
    void Client_TravelToLobby(APlayerController* PC);

    /** FTimerHandle */
    FTimerHandle BattleStartTimerHandle;
    FTimerHandle BattleTimerHandle;
    FTimerHandle ReturnLobbyTimerHandle;
};
