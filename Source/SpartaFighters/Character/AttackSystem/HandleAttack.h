#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "HandleAttack.generated.h"

UINTERFACE(BlueprintType)
class SPARTAFIGHTERS_API UHandleAttack : public UInterface
{
	GENERATED_BODY()
};

class SPARTAFIGHTERS_API IHandleAttack
{
	GENERATED_BODY()

public:
	virtual void PerformAttack() = 0;
};