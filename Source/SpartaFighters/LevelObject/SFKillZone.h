#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SFKillZone.generated.h"

UCLASS()
class SPARTAFIGHTERS_API ASFKillZone : public AActor
{
	GENERATED_BODY()
	
public:	
	ASFKillZone();

protected:
	UPROPERTY(VisibleAnywhere)
	class UBoxComponent* BoxComponent;

	UFUNCTION()
	void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};