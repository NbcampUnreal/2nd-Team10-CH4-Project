#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "DataTypes/MapInfo.h"
#include "MapInfoRow.generated.h"

USTRUCT(BlueprintType)
struct FMapInfoRow : public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    int32 MapID;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FMapInfo MapInfo;

    FMapInfoRow()
    {
		MapID = GetTypeHash(MapInfo.MapName);
    }
};