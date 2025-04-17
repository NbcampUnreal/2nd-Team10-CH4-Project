#include "Character/AttackSystem/EndAttackNotify.h"
#include "Character/SFCharacter.h"
#include "Character/Components/SkillComponent.h"

void UEndAttackNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (MeshComp && MeshComp->GetOwner())
	{
		AActor* Owner = MeshComp->GetOwner();
		ASFCharacter* Character = Cast<ASFCharacter>(Owner);

		if (Character)
		{
			if (Character->SkillComponent)
			{
				Character->SkillComponent->OnMontageEnded();
				Character->SkillComponent->ResetCombo();
			}
		}
	}
}
