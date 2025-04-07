

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Items/EquipItems/CosmeticItems/SFHat.h"
#include "Character/SFCharacter.h"
#include "SFTestHatEquip.generated.h"

UCLASS()
class SPARTAFIGHTERS_API ASFTestHatEquip : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASFTestHatEquip();

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Test")
	TSubclassOf<USFHat> HatClass;

	UPROPERTY(EditAnywhere, Category = "Test")
	ASFCharacter* TargetCharacter;

	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintCallable, Category = "Test")
	void EquipHat();
};
