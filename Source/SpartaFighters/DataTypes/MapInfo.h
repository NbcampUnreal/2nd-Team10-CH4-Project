#pragma once

#include "CoreMinimal.h"
#include "DataTypes/GameModeType.h"
#include "MapInfo.generated.h"

USTRUCT(BlueprintType)
struct FMapInfo
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString MapName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString LevelPath;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TSoftObjectPtr<UTexture2D> MapThumbnail;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EGameModeType GameMode;
};