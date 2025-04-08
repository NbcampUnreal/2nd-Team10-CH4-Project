#include "Character/AttackSystem/MeleeAttackNotify.h"
#include "Character/SFCharacter.h"

void UMeleeAttackNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (MeshComp && MeshComp->GetOwner())
	{
		AActor* Owner = MeshComp->GetOwner();
		ASFCharacter* Character = Cast<ASFCharacter>(Owner);

		if (Character)
		{
			Character->AttackTrace();
		}
	}
}
