#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AIMagicDamageArea.generated.h"

class UCapsuleComponent;

UCLASS()
class SPARTAFIGHTERS_API AAIMagicDamageArea : public AActor
{
	GENERATED_BODY()
	
public:
    AAIMagicDamageArea();

    UPROPERTY(VisibleAnywhere)
    UCapsuleComponent* DamageCapsule;

protected:
    virtual void BeginPlay() override;

    UPROPERTY(EditDefaultsOnly)
    float MagicDamage;

    UPROPERTY(EditDefaultsOnly)
    float DamageInterval;

    UPROPERTY(EditDefaultsOnly)
    float TotalDuration;

    FTimerHandle DamageTimerHandle;

    UFUNCTION()
    void ApplyDamageTick();

    UFUNCTION()
    void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
        bool bFromSweep, const FHitResult& SweepResult);

    TArray<AActor*> OverlappingActors;
};
