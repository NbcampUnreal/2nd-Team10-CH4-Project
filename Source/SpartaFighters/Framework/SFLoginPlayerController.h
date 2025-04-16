#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SFLoginPlayerController.generated.h"

UCLASS()
class SPARTAFIGHTERS_API ASFLoginPlayerController : public APlayerController
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;

public:
    ASFLoginPlayerController();

    UFUNCTION(Server, Reliable)
    void Server_RequestLogin(const FString& ID, const FString& Password);

    UFUNCTION(Client, Reliable)
    void Client_ReceiveLoginResult(bool bSuccess, const FString& Message);
    UFUNCTION(Client, Reliable)
    void Client_TravelToLobby();
};
