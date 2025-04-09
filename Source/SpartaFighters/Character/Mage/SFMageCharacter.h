#pragma once

#include "CoreMinimal.h"
#include "Character/SFCharacter.h"
#include "SFMageCharacter.generated.h"

UCLASS()
class SPARTAFIGHTERS_API ASFMageCharacter : public ASFCharacter
{
	GENERATED_BODY()
	
public:
	ASFMageCharacter();
	void CastingFireBall();

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	TSubclassOf<class AFireBall> ProjectileClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	UStaticMeshComponent* HatMesh;
};
