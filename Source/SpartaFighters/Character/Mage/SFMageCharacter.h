#pragma once

#include "CoreMinimal.h"
#include "Character/SFCharacter.h"
#include "SFMageCharacter.generated.h"

class UNiagaraComponent;

UCLASS()
class SPARTAFIGHTERS_API ASFMageCharacter : public ASFCharacter
{
	GENERATED_BODY()
	
public:
	ASFMageCharacter();

	void CastingFireBall();
	void TeleportForward();
	void DelayTeleport();
	void ShowTeleportCircleEffect();
	void ShowTeleportDustEffect();
	void ExecuteTelepoprt();

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	TSubclassOf<class AFireBall> ProjectileClass;
	UPROPERTY(EditDefaultsOnly, Category = "TeleportFX")
	TSubclassOf<AActor> TeleportCircleFXClass;
	UPROPERTY(EditDefaultsOnly, Category = "TeleportFX")
	TSubclassOf<AActor> TeleportDustFXClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	UStaticMeshComponent* HatMesh;

	UPROPERTY(EditAnywhere, Category = "Teleport")
	float TeleportDistance;

	FTimerHandle PrepareTeleportTimer;

};
