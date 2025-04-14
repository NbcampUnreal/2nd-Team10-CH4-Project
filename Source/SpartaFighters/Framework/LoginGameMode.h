#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "LoginGameMode.generated.h"

class ASFLoginPlayerController;
UCLASS()
class SPARTAFIGHTERS_API ALoginGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
    void HandleLogin(ASFLoginPlayerController* PlayerController, const FString& ID, const FString& Password);
};