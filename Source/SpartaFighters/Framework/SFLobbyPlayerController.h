#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SFLobbyPlayerController.generated.h"

UCLASS()
class SPARTAFIGHTERS_API ASFLobbyPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ASFLobbyPlayerController();

	UFUNCTION(Server, Reliable)
	void Server_SelectCharacter(FName CharacterRow, FName CommonItem, FName ExclusiveItem, FName CosmeticItem);

protected:
	virtual void BeginPlay() override;

};
