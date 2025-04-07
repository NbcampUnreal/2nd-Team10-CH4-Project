#pragma once

#include "CoreMinimal.h"
#include "Character/AttackSystem/HandleAttack.h"
#include "PunchAttack.generated.h"

UCLASS()
class SPARTAFIGHTERS_API UPunchAttack : public UObject, public IHandleAttack
{
	GENERATED_BODY()
	
public:
	virtual void PerformAttack() override;
};
