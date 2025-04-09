#include "FireBallAttack.h"
#include "Character/Mage/SFMageCharacter.h"

void UFireBallAttack::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (MeshComp && MeshComp->GetOwner())
	{
		AActor* Owner = MeshComp->GetOwner();
		ASFMageCharacter* Character = Cast<ASFMageCharacter>(Owner);

		if (Character)
		{
			Character->CastingFireBall();
		}
	}
}