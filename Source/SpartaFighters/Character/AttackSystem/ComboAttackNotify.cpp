#include "Character/AttackSystem/ComboAttackNotify.h"
#include "Character/SFCharacter.h"

void UComboAttackNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (MeshComp && MeshComp->GetOwner())
	{
		AActor* Owner = MeshComp->GetOwner();
		ASFCharacter* Character = Cast<ASFCharacter>(Owner);

		if (Character)
		{
			//Character->NextCombo();
		}
	}
}