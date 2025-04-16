#pragma once

#include "CoreMinimal.h"
#include "Character/SFCharacter.h"
#include "SFFighterCharacter.generated.h"


UCLASS()
class SPARTAFIGHTERS_API ASFFighterCharacter : public ASFCharacter
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;
};
