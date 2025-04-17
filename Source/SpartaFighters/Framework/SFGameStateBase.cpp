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

float ASFGameStateBase::GetRemainingBattleTime() const
{
    float ServerTime = GetWorld()->GetGameState()->GetServerWorldTimeSeconds();
    float Elapsed = ServerTime - BattleStartTime;
    return FMath::Max(0.f, BattleDuration - Elapsed);
}

float ASFGameStateBase::GetRemainingReturnToLobbyTime() const
{
    float ServerTime = GetWorld()->GetGameState()->GetServerWorldTimeSeconds();
    float Elapsed = ServerTime - StartReturnToLobbyTime;
    return FMath::Max(0.f, ReturnToLobbyTime - Elapsed);
}

float ASFGameStateBase::GetBattleDuration() const
{
    return BattleDuration;
}

float ASFGameStateBase::GetReturnToLobbyTime() const
{
    return ReturnToLobbyTime;
}

void ASFGameStateBase::SetBattleStartTime(float TimeInput)
{
    BattleStartTime = TimeInput;
}

void ASFGameStateBase::SetBattleDuration(float DurationInput)
{
    BattleDuration = DurationInput;
}

void ASFGameStateBase::SetReturnToLobbyTime(float TimeInput)
{
    ReturnToLobbyTime = TimeInput;
}

void ASFGameStateBase::SetStartReturnToLobbyTime(float TimeInput)
{
    StartReturnToLobbyTime = TimeInput;
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
    DOREPLIFETIME(ASFGameStateBase, BattleStartTime);
    DOREPLIFETIME(ASFGameStateBase, BattleDuration);
    DOREPLIFETIME(ASFGameStateBase, ReturnToLobbyTime);
    DOREPLIFETIME(ASFGameStateBase, StartReturnToLobbyTime);
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
