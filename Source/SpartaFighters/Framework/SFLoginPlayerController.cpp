#include "Framework/SFLoginPlayerController.h"
#include "Framework/SFGameInstanceSubsystem.h"
#include "Framework/LoginGameMode.h"
#include "Framework/SFCheatManager.h"
#include "UI/UIManager/UIManager.h"
#include "Kismet/GameplayStatics.h"

ASFLoginPlayerController::ASFLoginPlayerController()
{
    CheatClass = USFCheatManager::StaticClass();
}

void ASFLoginPlayerController::BeginPlay()
{
    Super::BeginPlay();

    if (IsLocalController())
    {
        if (USFGameInstanceSubsystem* Subsystem = GetGameInstance()->GetSubsystem<USFGameInstanceSubsystem>())
        {
            if (UUIManager* UIManager = Subsystem->GetUIManager())
            {
                UIManager->Init(this);
                UIManager->ShowLoginMenu();
            }
        }
    }
}

void ASFLoginPlayerController::Server_RequestLogin_Implementation(const FString& ID, const FString& Password)
{
    ALoginGameMode* LoginGM = Cast<ALoginGameMode>(UGameplayStatics::GetGameMode(this));
    if (LoginGM)
    {
        LoginGM->HandleLogin(this, ID, Password);
    }
}

void ASFLoginPlayerController::Client_ReceiveLoginResult_Implementation(bool bSuccess, const FString& Message)
{
    if (bSuccess)
    {
        UE_LOG(LogTemp, Log, TEXT("Login Success: %s"), *Message);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Login Failed: %s"), *Message);
    }
}

void ASFLoginPlayerController::Client_TravelToLobby_Implementation()
{
    UE_LOG(LogTemp, Warning, TEXT("Client is returning to Lobby!"));

    UGameplayStatics::OpenLevel(this, FName("/Game/SpartaFighters/Level/Menu/LobbyMenu"));
}
