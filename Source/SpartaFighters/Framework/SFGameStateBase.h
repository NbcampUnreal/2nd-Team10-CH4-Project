#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "SFGameStateBase.generated.h"

class ASFPlayerState;
UCLASS()
class SPARTAFIGHTERS_API ASFGameStateBase : public AGameStateBase
{
	GENERATED_BODY()

public:
    ASFGameStateBase();

    void SetWinner(ASFPlayerState* InWinner);

    ASFPlayerState* GetWinner() const { return WinnerPlayerState; }

    float GetRemainingBattleTime() const;
    float GetReturnToLobbyTime() const;
    void SetBattleStartTime(float TimeInput); 
    void SetBattleDuration(float DurationInput);
    void SetReturnToLobbyTime(float TimeInput);
    void SetStartReturnToLobbyTime(float TimeInput);


protected:
    UPROPERTY(ReplicatedUsing = OnRep_WinnerPlayerState)
    ASFPlayerState* WinnerPlayerState;

    UPROPERTY(Replicated)
    float BattleStartTime;

    UPROPERTY(Replicated)
    float BattleDuration;

    UPROPERTY(Replicated)
    float ReturnToLobbyTime;

    UPROPERTY(Replicated)
    float StartReturnToLobbyTime;

    UFUNCTION()
    void OnRep_WinnerPlayerState();

    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
    bool AreAllPlayersReady() const;
};