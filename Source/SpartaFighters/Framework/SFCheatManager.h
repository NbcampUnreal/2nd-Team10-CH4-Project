#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CheatManager.h"
#include "SFCheatManager.generated.h"

UCLASS()
class SPARTAFIGHTERS_API USFCheatManager : public UCheatManager
{
	GENERATED_BODY()
	
public:
	UFUNCTION(exec)
	void Cheat_ChangeMap(const FString& MapName);
};
