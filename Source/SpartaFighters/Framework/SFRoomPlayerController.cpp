#include "Framework/SFRoomPlayerController.h"
#include "Framework/SFGameInstanceSubsystem.h"
#include "Framework/SFGameInstance.h"
#include "Framework/SFBattleGameMode.h"
#include "Framework/SFGameStateBase.h"
#include "Framework/SFCheatManager.h"
#include "Framework/SFPlayerState.h"
#include "UI/UIManager/UIManager.h"

ASFRoomPlayerController::ASFRoomPlayerController()
{
    CheatClass = USFCheatManager::StaticClass();
}

void ASFRoomPlayerController::BeginPlay()
{
    Super::BeginPlay();

    if (IsLocalController())
    {
        if (USFGameInstanceSubsystem* Subsystem = GetGameInstance()->GetSubsystem<USFGameInstanceSubsystem>())
        {
            if (UUIManager* UIManager = Subsystem->GetUIManager())
            {
                UIManager->ShowRoomMenu();
            }
        }
        bShowMouseCursor = true;
        SetInputMode(FInputModeUIOnly());
    }
}

void ASFRoomPlayerController::Server_SelectCharacter_Implementation(FName CharacterRow/*, FName CommonItem, FName ExclusiveItem, FName CosmeticItem*/)
{
    UE_LOG(LogTemp,Warning, TEXT("Server_SelectCharacter_Implementation Called!"));
    if (ASFPlayerState* PS = GetPlayerState<ASFPlayerState>())
    {
        UE_LOG(LogTemp, Warning, TEXT("PS Should SetSelectedCharacterRow : %s"), *CharacterRow.ToString());
        PS->SetSelectedCharacterRow(CharacterRow);
        // PS->SetEquippedGear(CommonItem, ExclusiveItem, CosmeticItem); 
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("NULL : PS"));

    }
}

void ASFRoomPlayerController::Server_SetReady_Implementation(bool bReady)
{
    ASFPlayerState* PS = GetPlayerState<ASFPlayerState>();
    if(PS)
    {
        PS->bIsReady = bReady;
    }
}

void ASFRoomPlayerController::Server_RequestLevelChangeByMapName_Implementation(const FString& MapName)
{
    if (USFGameInstanceSubsystem* Subsystem = GetGameInstance()->GetSubsystem<USFGameInstanceSubsystem>())
    {
        Subsystem->ChangeLevelByMapName(MapName); 
    }
}