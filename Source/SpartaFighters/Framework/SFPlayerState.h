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

	UPROPERTY(Replicated= OnRep_PlayerUniqueUD)
	FString PlayerUniqueID;

	UPROPERTY(Replicated)
	FString SelectedCharacterName;

	UPROPERTY(Replicated)
	FString CharacterTexturePath;

	UPROPERTY(Replicated)
	TArray<FString> EquippedItems;

	UFUNCTION(Server, Unreliable)
	void Server_SetPlayerID(const FString& InPlayerID);

	UFUNCTION()
	void OnRep_PlayerUniqueID();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};