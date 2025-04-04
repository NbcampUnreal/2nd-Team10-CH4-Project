


#include "Items/EquipItems/ExclusiveItems/SFExclusiveBase.h"


USFExclusiveBase::USFExclusiveBase()
{
	RequiredClass = ESFRequiredClass::None; 
}

void USFExclusiveBase::OnEquipped_Implementation(AActor* Owner)
{
	Super::OnEquipped_Implementation(Owner);

	if (CanEquip(Owner))
	{
		//Todo
		//add skill
		/*if (Owner && AttachedSkill)
		{
			//skill logic
			
		}*/
	}
	else
	{
		//failed to equip
	}
}

void USFExclusiveBase::OnUnequipped_Implementation(AActor* Owner)
{
	Super::OnUnequipped_Implementation(Owner);

	//Todo
	//if (Owner && AttachedSkill)
	//{
	//	//delete skill
	//}
}


bool USFExclusiveBase::CanEquip(AActor* Owner)
{
	if (RequiredClass == ESFRequiredClass::None)
	{
		return true; 
	}


	
	 APawn* PawnOwner = Cast<APawn>(Owner);
	 if (PawnOwner)
	 {
		 //Todo
		//Get character class info by character component
	 	/*UCharacterClassComponent* ClassComponent = PawnOwner->GetComponentByClass<UCharacterClassComponent>();
	 	if (ClassComponent && ClassComponent->GetCurrentClass() == RequiredClass)
	 	{
	 		return true;
	 	}*/
	 }

	UE_LOG(LogTemp, Warning, TEXT("No Logic."));
	return false;
}