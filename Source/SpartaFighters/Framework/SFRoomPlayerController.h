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

protected:
	virtual void BeginPlay() override;
};
