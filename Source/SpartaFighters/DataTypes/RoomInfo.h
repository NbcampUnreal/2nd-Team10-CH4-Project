#pragma once

#include "CoreMinimal.h"
#include "DataTypes/GameModeType.h"
#include "DataTypes/PlayerInfo.h"
#include "RoomInfo.generated.h"

USTRUCT(BlueprintType)
struct FRoomInfo
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 RoomID;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString RoomName;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EGameModeType GameMode;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 CurrentPlayers;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 MaxPlayers;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bIsGameInProgress;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString OwnerPlayerID;
    UPROPERTY()
    FPlayerInfoArray PlayerList;

    bool operator==(const FRoomInfo& Other) const
    {
        return RoomID == Other.RoomID;
    }
};