#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "StatusContainerInterface.generated.h"

class UStatusContainerComponent;

UINTERFACE(MinimalAPI)
class UStatusContainerInterface : public UInterface
{
	GENERATED_BODY()
};

class SPARTAFIGHTERS_API IStatusContainerInterface
{
	GENERATED_BODY()

public:
	virtual UStatusContainerComponent* GetStatusContainerComponent() const = 0;
};
