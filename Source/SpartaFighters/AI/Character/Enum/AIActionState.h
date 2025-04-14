#pragma once

#include "CoreMinimal.h"
#include "AIActionState.generated.h"

UENUM(BlueprintType)
enum class EAIActionType : uint8
{
	None        UMETA(DisplayName = "None"),
	Attack      UMETA(DisplayName = "Attack"),
	Guard       UMETA(DisplayName = "Guard"),
	Evade       UMETA(DisplayName = "Evade"),
	Retreat     UMETA(DisplayName = "Retreat"),
	Chase		UMETA(DisplayName = "Chase")
};
