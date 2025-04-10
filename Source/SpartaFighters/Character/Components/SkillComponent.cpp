#include "Character/Components/SkillComponent.h"
#include "GameFramework/Character.h"
#include "Animation/AnimInstance.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"


USkillComponent::USkillComponent()
{

}

void USkillComponent::Initialize(UDataTable* InSkillDataTable, UStateComponent* InStateComp, ACharacter* Character)
{
	SkillDataTable = InSkillDataTable;
	StateComponent = InStateComp;
	OwnerCharacter = Character;
}

void USkillComponent::HandleInputBasicAttack()
{
	if (!IsValid(OwnerCharacter) || !IsValid(SkillDataTable)) return;


	if (OwnerCharacter->HasAuthority())
	{
		StateComponent->UpdateState(OwnerCharacter);
		Multicast_HandleBasicAttack(StateComponent->GetState());
	}
	else
	{
		Server_HandleBasicAttack();
	}
}

void USkillComponent::Server_HandleBasicAttack_Implementation()
{
	if (StateComponent && OwnerCharacter)
	{
		StateComponent->UpdateState(OwnerCharacter);
	}

	Multicast_HandleBasicAttack(StateComponent->GetState());
}

void USkillComponent::Multicast_HandleBasicAttack_Implementation(ECharacterState State)
{
	HandleBasicAttack(State);
}

void USkillComponent::HandleBasicAttack(ECharacterState CurrentState)
{
	if (!StateComponent || StateComponent->IsInAction()) return;

	FName RowName = TEXT("BaseAttack_1");

	switch (CurrentState)
	{
	case ECharacterState::Moving:
		RowName = FName("MoveBaseAttack");
		break;
	case ECharacterState::InAir:
		RowName = FName("JumpBaseAttack");
		break;
	case ECharacterState::Crouching:
		RowName = FName("CrouchBaseAttack");
		break;
	}

	PlayAnimMontage(RowName);
}

void USkillComponent::HandleInputSkillAttack()
{
	// TO DO : Handling

}

void USkillComponent::PlayAnimMontage(const FName& RowName)
{
	CurrentSkillData = SkillDataTable->FindRow<FSkillDataRow>(RowName, TEXT("SkillLookup"));
	if (!CurrentSkillData || !CurrentSkillData->SkillMontage) return;

	if (UAnimInstance* Anim = OwnerCharacter->GetMesh()->GetAnimInstance())
	{
		Anim->Montage_Play(CurrentSkillData->SkillMontage);

		FOnMontageEnded EndDelegate;
		EndDelegate.BindUObject(this, &USkillComponent::OnMontageEnded);
		Anim->Montage_SetEndDelegate(EndDelegate, CurrentSkillData->SkillMontage);

		StateComponent->SetIsInAction(true);
	}
}

void USkillComponent::OnMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (StateComponent)
	{
		StateComponent->SetIsInAction(false);
	}
}

void USkillComponent::PerformAttackTrace()
{
	if (!CurrentSkillData || !OwnerCharacter) return;

	FVector SocketLocation = OwnerCharacter->GetMesh()->GetSocketLocation(CurrentSkillData->SocketLocation);

	const float TraceLength = CurrentSkillData->TraceLength;
	const float TraceRadius = CurrentSkillData->TraceRadius;

	FVector StartLocation = SocketLocation - OwnerCharacter->GetActorForwardVector() * (TraceLength / 2.0f);
	FVector EndLocation = SocketLocation + OwnerCharacter->GetActorForwardVector() * (TraceLength / 2.0f);

	FHitResult HitResult;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(OwnerCharacter);

	const bool bHit = OwnerCharacter->GetWorld()->SweepSingleByChannel(
		HitResult,
		StartLocation,
		EndLocation,
		FQuat::Identity,
		ECC_Pawn,
		FCollisionShape::MakeCapsule(TraceRadius, TraceRadius),
		Params
	);

	if (OwnerCharacter->HasAuthority() && bHit && HitResult.GetActor())
	{
		UGameplayStatics::ApplyPointDamage(
			HitResult.GetActor(),
			CurrentSkillData->AttackPower,
			EndLocation - StartLocation,
			HitResult,
			OwnerCharacter->GetController(),
			OwnerCharacter,
			UDamageType::StaticClass()
		);
	}

#if WITH_EDITOR
	DrawDebugCapsule(
		OwnerCharacter->GetWorld(),
		(StartLocation + EndLocation) / 2.0f,
		TraceLength / 2.0f,
		TraceRadius,
		FRotationMatrix::MakeFromZ(EndLocation - StartLocation).ToQuat(),
		bHit ? FColor::Red : FColor::Green,
		false, 1.0f
	);
#endif
}
