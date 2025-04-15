

#pragma once

#include "CoreMinimal.h"
#include "DistanceState.generated.h"


UENUM(BlueprintType)
enum class EDistanceType : uint8
{
    None        UMETA(DisplayName = "None"),
    Melee       UMETA(DisplayName = "Melee"),
    Ranged      UMETA(DisplayName = "Ranged"),
    TooFar      UMETA(DisplayName = "TooFar")
};