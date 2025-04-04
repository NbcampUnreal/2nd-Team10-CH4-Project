#pragma once

#include "CoreMinimal.h"
#include "GameModeType.generated.h"

UENUM(BlueprintType)
enum class EGameModeType : uint8
{
    Cooperative UMETA(DisplayName = "Cooperative"),
    Battle      UMETA(DisplayName = "Battle"),
    Single      UMETA(DisplayName = "Single")
};