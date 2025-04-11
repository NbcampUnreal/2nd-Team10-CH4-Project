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

protected:
    UPROPERTY(ReplicatedUsing = OnRep_WinnerPlayerState)
    ASFPlayerState* WinnerPlayerState;

    UFUNCTION()
    void OnRep_WinnerPlayerState();

    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};