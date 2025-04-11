#include "Framework/LoginGameMode.h"
#include "Framework/SFLoginPlayerController.h"

void ALoginGameMode::HandleLogin(ASFLoginPlayerController* PlayerController, const FString& ID, const FString& Password)
{
    if (ID == "Test" && Password == "1234")
    {
        PlayerController->Client_ReceiveLoginResult(true, TEXT("Welcome, Test!"));

        // PlayerController->ClientTravel("/Game/Maps/Lobby", TRAVEL_Absolute);
    }
    else
    {
        PlayerController->Client_ReceiveLoginResult(false, TEXT("Invalid ID or Password."));
    }
}