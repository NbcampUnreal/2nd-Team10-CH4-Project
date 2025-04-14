#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SFRoomPlayerController.generated.h"

UCLASS()
class SPARTAFIGHTERS_API ASFRoomPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ASFRoomPlayerController();

	virtual void BeginPlay() override;

	UFUNCTION(Server, Reliable)
	void Server_SetReady(bool bReady);
	UFUNCTION(Server, Reliable)
	void Server_RequestLevelChangeByMapName(const FString& MapName);
	UFUNCTION(Server, Reliable)
	void Server_SelectCharacter(FName CharacterRow/*, FName CommonItem, FName ExclusiveItem, FName CosmeticItem*/);
};
