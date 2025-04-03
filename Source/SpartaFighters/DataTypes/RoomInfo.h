#pragma once

#include "CoreMinimal.h"
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
    FString GameMode;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString MapName;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 CurrentPlayers;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 MaxPlayers;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bIsGameInProgress;
};