#pragma once

#include "CoreMinimal.h"
#include "Items/SFItemBase.h"
#include "Character/SFCharacter.h"
#include "Net/UnrealNetwork.h"
#include "SFConsumableBase.generated.h"

UCLASS()
class SPARTAFIGHTERS_API USFConsumableBase : public USFItemBase
{
	GENERATED_BODY()
	
public: 
	USFConsumableBase();
	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Consumable")
	void Server_ApplyConsumableEffect(ASFCharacter* InPlayerCharacter);
	virtual void Server_ApplyConsumableEffect_Implementation(ASFCharacter* InPlayerCharacter);



};
