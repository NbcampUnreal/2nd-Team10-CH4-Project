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
	if (!IsValid(OwnerCharacter) || !IsValid(SkillDataTable)) return;

	if (OwnerCharacter->HasAuthority())
	{
		StateComponent->UpdateState(OwnerCharacter);
		Multicast_HandleSkillAttack(StateComponent->GetState());
	}
	else
	{
		Server_HandleSkillAttack();
	}
}

void USkillComponent::Server_HandleSkillAttack_Implementation()
{
	if (StateComponent && OwnerCharacter)
	{
		StateComponent->UpdateState(OwnerCharacter);
	}

	Multicast_HandleSkillAttack(StateComponent->GetState());
}

void USkillComponent::Multicast_HandleSkillAttack_Implementation(ECharacterState State)
{
	HandleSkillAttack(State);
}

void USkillComponent::HandleSkillAttack(ECharacterState CurrentState)
{
	FName RowName = TEXT("IdleSkill");

	switch (CurrentState)
	{
	case ECharacterState::Moving:
		RowName = FName("MoveSkill");
		break;
	case ECharacterState::InAir:
		RowName = FName("JumpSkill");
		break;
	case ECharacterState::Crouching:
		RowName = FName("CrouchSkill");
		break;
	}

	PlayAnimMontage(RowName);
}

void USkillComponent::HandleInputRoll()
{
	if (!IsValid(OwnerCharacter) || !IsValid(SkillDataTable)) return;

	if (OwnerCharacter->HasAuthority())
	{
		Multicast_HandleRoll(StateComponent->GetState());
	}
	else
	{
		Server_HandleRoll();
	}
}

void USkillComponent::Multicast_HandleRoll_Implementation(ECharacterState State)
{
	HandleRoll(State);
}

void USkillComponent::Server_HandleRoll_Implementation()
{
	Multicast_HandleRoll(StateComponent->GetState());
}

void USkillComponent::HandleRoll(ECharacterState CurrentState)
{
	FName RowName = TEXT("SpecialMoveSkill");
	PlayAnimMontage(RowName);
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

	ClearHitActors();
}

void USkillComponent::PerformAttackTrace()
{
	if (!CurrentSkillData || !OwnerCharacter) return;

	FVector SocketLocation = OwnerCharacter->GetMesh()->GetSocketLocation(CurrentSkillData->SocketLocation);

	const float TraceLength = CurrentSkillData->TraceLength;
	const float TraceRadius = CurrentSkillData->TraceRadius;

	FHitResult HitResult;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(OwnerCharacter);

	TArray<FHitResult> HitResults;
	bool bHit = OwnerCharacter->GetWorld()->SweepMultiByChannel(
		HitResults,
		SocketLocation,
		SocketLocation,
		FQuat::Identity,
		ECC_Pawn,
		FCollisionShape::MakeCapsule(TraceRadius, TraceLength),
		Params
	);

	if (OwnerCharacter->HasAuthority() && bHit)
	{
		for (const FHitResult& Hit : HitResults)
		{
			AActor* HitActor = Hit.GetActor();
			if (!HitActor) continue;

			// 중복 피격 방지
			if (AlreadyHitActors.Contains(HitActor)) continue;
			AlreadyHitActors.Add(HitActor);

			UGameplayStatics::ApplyPointDamage(
				HitActor,
				CurrentSkillData->AttackPower,
				SocketLocation,
				Hit,
				OwnerCharacter->GetController(),
				OwnerCharacter,
				UDamageType::StaticClass()
			);
		}
	}

#if WITH_EDITOR
	FVector Forward = OwnerCharacter->GetActorForwardVector();
	FVector Start = SocketLocation - Forward * (TraceLength / 2);
	FVector End = SocketLocation + Forward * (TraceLength / 2);
	FQuat Rotation = FRotationMatrix::MakeFromZ(Forward).ToQuat();

	DrawDebugCapsule(
		OwnerCharacter->GetWorld(),
		(Start + End) / 2,
		TraceLength / 2,
		TraceRadius,
		Rotation,
		bHit ? FColor::Red : FColor::Green,
		false, 1.0f
	);
#endif
}

void USkillComponent::ClearHitActors()
{
	AlreadyHitActors.Empty();
}
