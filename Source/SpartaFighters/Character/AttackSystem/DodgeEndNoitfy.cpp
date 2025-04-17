#include "Character/AttackSystem/DodgeEndNoitfy.h"
#include "Character/SFCharacter.h"
#include "Character/Components/StateComponent.h"


void UDodgeEndNoitfy::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (MeshComp && MeshComp->GetOwner())
	{
		AActor* Owner = MeshComp->GetOwner();
		ASFCharacter* Character = Cast<ASFCharacter>(Owner);

		if (Character)
		{
			if (Character->StateComponent)
			{
				Character->StateComponent->ResetSpecialState();
			}
		}
	}
}
