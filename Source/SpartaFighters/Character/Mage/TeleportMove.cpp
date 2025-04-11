#include "Character/Mage/TeleportMove.h"
#include "Character/Mage/SFMageCharacter.h"

void UTeleportMove::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (MeshComp && MeshComp->GetOwner())
	{
		AActor* Owner = MeshComp->GetOwner();
		ASFMageCharacter* Character = Cast<ASFMageCharacter>(Owner);

		if (Character)
		{
			Character->TeleportForward();
		}
	}
}