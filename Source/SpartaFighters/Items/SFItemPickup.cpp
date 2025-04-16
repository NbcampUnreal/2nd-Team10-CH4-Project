#include "Items/SFItemPickup.h"
#include "Components/SphereComponent.h"
#include "Items/ConsumableItems/SFConsumableBase.h"
#include "Character/SFCharacter.h"
#include "Net/UnrealNetwork.h"


ASFItemPickup::ASFItemPickup()
{
	OverlapSphere = CreateDefaultSubobject<USphereComponent>(TEXT("OverlapSphere"));
	OverlapSphere->SetSphereRadius(100.0f);
	RootComponent = OverlapSphere;
	SetReplicates(true);//Replicate actore

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
		PickupItemInstance = NewObject<USFConsumableBase>(GetWorld(), ItemClass);
		if (PickupItemInstance)
		{
			UE_LOG(LogTemp, Warning, TEXT("%s successfully generated: %s (%s)"), *GetName(), *PickupItemInstance->ItemName.ToString(), *UEnum::GetValueAsString(PickupItemInstance->ItemType));
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("%s failed generation: unable to generate pick up by ItemClass."), *GetName());
		}
	}
	else if(!ItemClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s generated: ItemClass not defined."), *GetName());
	}
}

void ASFItemPickup::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ASFCharacter* PlayerCharacter = Cast<ASFCharacter>(OtherActor);
	if (PlayerCharacter && PlayerCharacter->HasAuthority())
	{
		UE_LOG(LogTemp, Warning, TEXT("%s picked up and used by server: collide with %s "), *PickupItemInstance->ItemName.ToString(), *PlayerCharacter->GetName());

		if (PickupItemInstance->IsA(USFConsumableBase::StaticClass()))
		{
			USFConsumableBase* ConsumableItem = Cast<USFConsumableBase>(PickupItemInstance);
			ConsumableItem->Server_ApplyConsumableEffect(PlayerCharacter); //Server RPC
			UE_LOG(LogTemp, Warning, TEXT("%s used by %s"), *ConsumableItem->ItemName.ToString(), *PlayerCharacter->GetName());
			Destroy();
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("%s is not a consumable item."), *PickupItemInstance->ItemName.ToString());
		}

		PickupItemInstance->OnItemAcquired();
	}
}

void ASFItemPickup::Server_UsePickup_Implementation(ASFCharacter* PlayerCharacter)
{
	if (PlayerCharacter && PickupItemInstance && HasAuthority())
	{
		UE_LOG(LogTemp, Warning, TEXT("%s picked up and used by server: collide with %s "), *PickupItemInstance->ItemName.ToString(), *PlayerCharacter->GetName());

		if (PickupItemInstance->IsA(USFConsumableBase::StaticClass()))
		{
			USFConsumableBase* ConsumableItem = Cast<USFConsumableBase>(PickupItemInstance);
			ConsumableItem->Server_ApplyConsumableEffect(PlayerCharacter); //Server RPC
			UE_LOG(LogTemp, Warning, TEXT("%s used by %s"), *ConsumableItem->ItemName.ToString(), *PlayerCharacter->GetName());
			Destroy();
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("%s is not a consumable item."), *PickupItemInstance->ItemName.ToString());
		}

		PickupItemInstance->OnItemAcquired(); 
	}
}


void ASFItemPickup::SetPickupMesh(UStaticMesh* NewMesh)
{
	if (PickupMesh)
	{
		PickupMesh->SetStaticMesh(NewMesh);
	}
}

void ASFItemPickup::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASFItemPickup, PickupItemInstance);
	DOREPLIFETIME(ASFItemPickup, OverlapSphere);
	DOREPLIFETIME(ASFItemPickup, PickupMesh);
}