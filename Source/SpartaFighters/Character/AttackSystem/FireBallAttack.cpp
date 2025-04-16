#include "FireBallAttack.h"
#include "Character/SFCharacter.h"
#include "Character/Mage/SFMageCharacter.h"

void UFireBallAttack::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (MeshComp && MeshComp->GetOwner())
	{
		AActor* Owner = MeshComp->GetOwner();
		ASFCharacter* Character = Cast<ASFCharacter>(Owner);

		if (Character && Character->HasAuthority())
		{
			Character->SpawnFireBall();
		}
	}
}