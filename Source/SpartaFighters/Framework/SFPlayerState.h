#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "SFPlayerState.generated.h"

class ASFGameStateBase;
UCLASS()
class SPARTAFIGHTERS_API ASFPlayerState : public APlayerState
{
	GENERATED_BODY()
	
public:
	ASFPlayerState();

	UPROPERTY(ReplicatedUsing = OnRep_bIsReady)
	bool bIsReady = false;

	UPROPERTY(ReplicatedUsing = OnRep_bIsRoomOwner)
	bool bIsRoomOwner = false;

	UPROPERTY(Replicated)
	bool bIsAI = false;

	UPROPERTY(Replicated)
	FName CharacterRowName;

	UPROPERTY(Replicated)
	FString CustomPlayerID;

	FString GetUniqueID() const;

public:
	void AddDeathCount();
	int32 GetDeathCount() const { return DeathCount; }

protected:
	UPROPERTY(Replicated)
	int32 DeathCount = 0;

	UFUNCTION()
	void OnRep_bIsReady();

	UFUNCTION()
	void OnRep_bIsRoomOwner();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};