#include "Framework/SFPlayerState.h"
#include "Framework/SFGameStateBase.h"
#include "Net/UnrealNetwork.h"

ASFPlayerState::ASFPlayerState()
{
	bReplicates = true;
}

void ASFPlayerState::Server_SetPlayerInfoID_Implementation(const FString& InPlayerID)
{
    PlayerInfo.PlayerID = InPlayerID;
}

void ASFPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ASFPlayerState, PlayerInfo);
}

const FPlayerInfo* ASFPlayerState::GetMyPlayerInfo() const
{
    ASFGameStateBase* GS = GetWorld() ? GetWorld()->GetGameState<ASFGameStateBase>() : nullptr;
    if (!GS)
    {
        return nullptr;
    }

    const TArray<FPlayerInfo>& PlayerList = GS->GetPlayerList(); 

    for (const FPlayerInfo& Info : PlayerList)
    {
        if (Info.PlayerID == PlayerInfo.PlayerID)
        {
            return &Info;
        }
    }

    return nullptr; 
}