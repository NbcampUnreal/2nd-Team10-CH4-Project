#include "LevelObject/SFKillZone.h"
#include "Components/BoxComponent.h"
#include "Character/SFCharacter.h"

ASFKillZone::ASFKillZone()
{
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	RootComponent = BoxComponent;
	BoxComponent->SetCollisionProfileName("OverlapAllDynamic");
	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &ASFKillZone::OnOverlap);
}

void ASFKillZone::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (ASFCharacter* Char = Cast<ASFCharacter>(OtherActor))
	{
		Char->DieImmediately();
	}
}