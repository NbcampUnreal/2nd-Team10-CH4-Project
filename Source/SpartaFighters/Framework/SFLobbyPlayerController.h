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

protected:
    virtual void BeginPlay() override;

};
