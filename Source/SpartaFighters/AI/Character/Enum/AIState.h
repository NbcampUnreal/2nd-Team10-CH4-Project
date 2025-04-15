

#pragma once

#include "CoreMinimal.h"
#include "AIState.generated.h"

UENUM(BlueprintType)
enum class EAIState : uint8
{
	None        UMETA(DisplayName = "None"),
	Idle      UMETA(DisplayName = "Idle"),
	Jump       UMETA(DisplayName = "Jump"),
	Crouch       UMETA(DisplayName = "Crouch")
};