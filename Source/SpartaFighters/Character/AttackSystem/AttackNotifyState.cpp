#include "Character/AttackSystem/AttackNotifyState.h"
#include "GameFramework/Character.h"
#include "Character/Components/SkillComponent.h"

void UAttackNotifyState::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	float FrameDeltaTime)
{
	if (!MeshComp) return;

	ACharacter* Character = Cast<ACharacter>(MeshComp->GetOwner());
	if (!Character) return;

	if (USkillComponent* SkillComp = Character->FindComponentByClass<USkillComponent>())
	{
		SkillComp->PerformAttackTrace(); 
	}
}
