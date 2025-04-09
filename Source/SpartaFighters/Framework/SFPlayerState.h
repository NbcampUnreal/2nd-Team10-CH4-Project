#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "DataTypes/PlayerInfo.h"
#include "SFPlayerState.generated.h"

class ASFGameStateBase;
UCLASS()
class SPARTAFIGHTERS_API ASFPlayerState : public APlayerState
{
	GENERATED_BODY()
	
public:
	ASFPlayerState();

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Player")
	FPlayerInfo PlayerInfo;

	UFUNCTION(BlueprintCallable, Category = "Player")
	FString GetPlayerInfoID() const { return PlayerInfo.PlayerID; }

	UFUNCTION(Server, Reliable)
	void Server_SetPlayerInfoID(const FString& InPlayerID);

	const FPlayerInfo* GetMyPlayerInfo() const;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
