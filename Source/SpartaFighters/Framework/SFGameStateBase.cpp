#include "Framework/SFGameStateBase.h"
#include "Net/UnrealNetwork.h"
#include "Framework/SFPlayerState.h"

ASFGameStateBase::ASFGameStateBase()
{
}

void ASFGameStateBase::SetWinner(ASFPlayerState* InWinner)
{
    if (HasAuthority())
    {
        WinnerPlayerState = InWinner;

        if (InWinner)
        {
            UE_LOG(LogTemp, Log, TEXT("Winner is: %s"), *InWinner->GetPlayerName());
        }
    }
}

void ASFGameStateBase::OnRep_WinnerPlayerState()
{
    if (WinnerPlayerState)
    {
        FString WinnerName = WinnerPlayerState->GetPlayerName();
        UE_LOG(LogTemp, Log, TEXT("Client received winner info: %s"), *WinnerName);

        // TODO : Show UI or Play Animation
        // ex) UBattleResultWidget->ShowWinner(WinnerName);
    }
}

void ASFGameStateBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(ASFGameStateBase, WinnerPlayerState);
}

bool ASFGameStateBase::AreAllPlayersReady() const
{
    for (APlayerState* PS : PlayerArray)
    {
        ASFPlayerState* SFPS = Cast<ASFPlayerState>(PS);
        if (!SFPS) continue;

        if (!SFPS->bIsAI && !SFPS->bIsReady)
        {
            return false;
        }
    }
    return true;
}
