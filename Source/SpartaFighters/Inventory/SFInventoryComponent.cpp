


#include "Inventory/SFInventoryComponent.h"
#include "Character/SFCharacter.h"

// Sets default values for this component's properties
USFInventoryComponent::USFInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void USFInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void USFInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void USFInventoryComponent::UpdateData()
{
	//get game instance
	UGameInstance* Instance = GetWorld()->GetGameInstance();
	if (Instance)
	{
		/*USFGameInstance* SFGameInstance = Cast<USFGameInstance>(Instance);
		if (SFGameInstance)
		{
			SFGameInstance->Has... = Potion;
		}*/
	}
}

void USFInventoryComponent::UseConsumable()
{
	//subtract count by 1 if count is not 1
	//UpdateData(); at the end
	AActor* PlayerCharacter = GetOwner();
	if (PlayerCharacter)
	{
		ASFCharacter* SFPlayerCharacter = Cast<ASFCharacter>(PlayerCharacter);
		if (!SFPlayerCharacter) //|| ItemCount <= 0)
		{
			return;
		}
		//ItemCount--;
		//SFPlayerCharacter->ApplyConsumableEffect();
		UpdateData();
	}
}

void USFInventoryComponent::AddItem(USFItemBase* Item)
{
	//add Item to array
	//add count if Item is already in the array
	UpdateData(); 
}

void USFInventoryComponent::RemoveItem(USFItemBase* Item)
{
	//delete item from array if count is 1
	//else UseConsumable();
	UpdateData();
}