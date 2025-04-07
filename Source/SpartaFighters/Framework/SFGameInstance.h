#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "SFGameInstance.generated.h"

class UUIManagerSettings;

UCLASS()
class SPARTAFIGHTERS_API USFGameInstance : public UGameInstance
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category = "UI")
	UUIManagerSettings* UISettings;

public:
	UUIManagerSettings* GetUISettings() const { return UISettings; }
	
};
