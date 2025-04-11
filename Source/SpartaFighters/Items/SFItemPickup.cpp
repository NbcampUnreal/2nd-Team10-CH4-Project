#include "Items/SFItemPickup.h"
#include "Components/SphereComponent.h"
#include "Items/ConsumableItems/SFConsumableBase.h"
#include "Character/SFCharacter.h"


ASFItemPickup::ASFItemPickup()
{
	OverlapSphere = CreateDefaultSubobject<USphereComponent>(TEXT("OverlapSphere"));
	OverlapSphere->SetSphereRadius(100.0f);
	RootComponent = OverlapSphere;

	PickupMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PickupMesh"));
	PickupMesh->SetupAttachment(RootComponent);

	OverlapSphere->OnComponentBeginOverlap.AddDynamic(this, &ASFItemPickup::OnOverlapBegin);

	PickupItemInstance = nullptr;

}


void ASFItemPickup::BeginPlay()
{
	Super::BeginPlay();
	if (ItemClass)
	{
		PickupItemInstance = NewObject<USFConsumableBase>(GetTransientPackage(), ItemClass);
		if (PickupItemInstance)
		{
			UE_LOG(LogTemp, Warning, TEXT("%s successfully generated: %s (%s)"), *GetName(), *PickupItemInstance->ItemName.ToString(), *UEnum::GetValueAsString(PickupItemInstance->ItemType));
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("%s failed generation: unable to generate pick up by ItemClass."), *GetName());
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("%s generated: ItemClass not defined."), *GetName());
	}
}

void ASFItemPickup::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ASFCharacter* PlayerCharacter = Cast<ASFCharacter>(OtherActor);
	if (PlayerCharacter && PlayerCharacter->IsLocallyControlled()) 
	{
		if (PickupItemInstance)
		{
			UE_LOG(LogTemp, Warning, TEXT("%s picked up: collide with %s "), *PickupItemInstance->ItemName.ToString(), *PlayerCharacter->GetName());

			if (PickupItemInstance->IsA(USFConsumableBase::StaticClass()))
			{
				USFConsumableBase* ConsumableItem = Cast<USFConsumableBase>(PickupItemInstance);
				ConsumableItem->ApplyConsumableEffect(PlayerCharacter);
				UE_LOG(LogTemp, Warning, TEXT("%s : %s"), *PlayerCharacter->GetName(), *ConsumableItem->ItemName.ToString());
				Destroy();
			}
			else
			{
			}

			//use if additional logic required
			PickupItemInstance->OnItemAcquired();
		}
	}
}


void ASFItemPickup::SetPickupMesh(UStaticMesh* NewMesh)
{
	if (PickupMesh)
	{
		PickupMesh->SetStaticMesh(NewMesh);
	}
}