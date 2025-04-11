#include "Framework/RoomGameMode.h"
#include "Framework/SFGameStateBase.h"
#include "Framework/SFPlayerState.h"

void ARoomGameMode::PostLogin(APlayerController* NewPlayer)
{
    Super::PostLogin(NewPlayer);

    ASFPlayerState* PS = NewPlayer->GetPlayerState<ASFPlayerState>();
    if (PS)
    {
        PS->CustomPlayerID = FString::Printf(TEXT("Player_%d"), FMath::Rand());

        if (GameState->PlayerArray.Num() == 1)
        {
            PS->bIsRoomOwner = true;
        }
    }
}