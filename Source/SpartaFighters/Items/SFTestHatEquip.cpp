
#include "Items/SFTestHatEquip.h"


// Sets default values
ASFTestHatEquip::ASFTestHatEquip()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void ASFTestHatEquip::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASFTestHatEquip::EquipHat()
{
	if (HatClass && TargetCharacter)
	{
		USFHat* HatInstance = NewObject<USFHat>(TargetCharacter, HatClass);
		if (HatInstance)
		{
			HatInstance->ApplyCosmetic(TargetCharacter);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("HatClass or TargetCharacter is not setted"));
	}
}

