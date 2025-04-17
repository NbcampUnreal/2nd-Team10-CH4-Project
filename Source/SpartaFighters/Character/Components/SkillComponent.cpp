#include "Character/Components/SkillComponent.h"
#include "GameFramework/Character.h"
#include "Animation/AnimInstance.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"
#include "Character/SFCharacter.h"
#include "Character/Components/StatusComponent.h"

#include "Common/SkillDamageEvent.h"


USkillComponent::USkillComponent()
{

}

void USkillComponent::Initialize(UDataTable* InSkillDataTable, UStateComponent* InStateComp, ASFCharacter* Character)
{
	SkillDataTable = InSkillDataTable;
	StateComponent = InStateComp;
	OwnerCharacter = Character;
}

void USkillComponent::HandleInputBasicAttack()
{
	if (!IsValid(OwnerCharacter) || !IsValid(SkillDataTable)) return;

	Server_HandleBasicAttack();
}

void USkillComponent::Server_HandleBasicAttack_Implementation()
{
	if (!StateComponent || StateComponent->IsInAction()) return;
	UE_LOG(LogTemp, Warning, TEXT("[Server] Handle Basic Attack"));

	FName RowName = TEXT("BaseAttack_1");

	switch (StateComponent->GetState())
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
	case ECharacterState::Idle:
		ComboCount++;
		UE_LOG(LogTemp, Warning, TEXT("ComboCount : %d"), ComboCount);
		if (ComboCount > MaxComboCount) return;
		RowName = *FString::Printf(TEXT("BaseAttack_%d"), ComboCount);
		break;
	}

	CurrentSkillData = SkillDataTable->FindRow<FSkillDataRow>(RowName, TEXT("SkillLookup"));

	Multicast_HandleBasicAttack(*CurrentSkillData);
}

void USkillComponent::Multicast_HandleBasicAttack_Implementation(FSkillDataRow Data)
{
	HandleBasicAttack(&Data);
}

void USkillComponent::HandleBasicAttack(FSkillDataRow* Data)
{
	PlayAnimMontage(Data);
	UGameplayStatics::PlaySoundAtLocation(this, OwnerCharacter->AttackSound, OwnerCharacter->GetActorLocation());
}


void USkillComponent::HandleInputSkillAttack()
{
	if (!IsValid(OwnerCharacter) || !IsValid(SkillDataTable)) return;

	Server_HandleSkillAttack();
}

void USkillComponent::Server_HandleSkillAttack_Implementation()
{
	if (!StateComponent || StateComponent->IsInAction()) return;
	UE_LOG(LogTemp, Warning, TEXT("Handle Skill Attack"));

	FName RowName = TEXT("IdleSkill");

	switch (StateComponent->GetState())
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

	CurrentSkillData = SkillDataTable->FindRow<FSkillDataRow>(RowName, TEXT("SkillLookup"));

	Multicast_HandleSkillAttack(*CurrentSkillData);
}

void USkillComponent::Multicast_HandleSkillAttack_Implementation(FSkillDataRow Data)
{
	HandleSkillAttack(&Data);
}

void USkillComponent::HandleSkillAttack(FSkillDataRow* Data)
{
	UStatusComponent* StatusComponet = OwnerCharacter->GetStatusComponent();
	if (!IsValid(StatusComponet))
	{
		return;
	}

	if (!Data || !Data->SkillMontage) return;


	float CurrentMP = StatusComponet->GetStatusValue(EStatusType::CurMP);
	if (CurrentMP < Data->MPCost)
	{
		UE_LOG(LogTemp, Log, TEXT("Current MP is Not Enought!! CurrentMP : %f, MPCost : %f"), CurrentMP, Data->MPCost);
	}
	else
	{
		StatusComponet->ModifyMP(-Data->MPCost);

		PlayAnimMontage(Data);
		UGameplayStatics::PlaySoundAtLocation(this, OwnerCharacter->SkillSound, OwnerCharacter->GetActorLocation());
	}
}

void USkillComponent::HandleInputDodge()
{
	if (!IsValid(OwnerCharacter) || !IsValid(SkillDataTable)) return;

	Server_HandleDodge();
}

void USkillComponent::Server_HandleDodge_Implementation()
{
	FName RowName = TEXT("Dodge");
	StateComponent->SetIsInAction(true);
	StateComponent->SetSpecialState(ECharacterSpecialState::Invincible);
	CurrentSkillData = SkillDataTable->FindRow<FSkillDataRow>(RowName, TEXT("SkillLookup"));

	Multicast_HandleDodge(*CurrentSkillData);
}

void USkillComponent::Multicast_HandleDodge_Implementation(FSkillDataRow Data)
{
	HandleDodge(&Data);
}

void USkillComponent::HandleDodge(FSkillDataRow* Data)
{
	PlayAnimMontage(Data);
}

void USkillComponent::PlayAnimMontage()
{
	if (!CurrentSkillData || !CurrentSkillData->SkillMontage) return;

	if (UAnimInstance* Anim = OwnerCharacter->GetMesh()->GetAnimInstance())
	{
		Anim->Montage_Play(CurrentSkillData->SkillMontage);

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
	ComboCount = 0;
}

void USkillComponent::PlayAnimMontage(FSkillDataRow* Data)
{
	if (!Data || !Data->SkillMontage) return;

	if (UAnimInstance* Anim = OwnerCharacter->GetMesh()->GetAnimInstance())
	{
		Anim->Montage_Play(Data->SkillMontage);

		//FOnMontageEnded EndDelegate;
		//EndDelegate.BindUObject(this, &USkillComponent::OnMontageEnded);
		//Anim->Montage_SetEndDelegate(EndDelegate, Data->SkillMontage);

		StateComponent->SetIsInAction(true);
	}
}

void USkillComponent::OnMontageEnded()
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

			FSkillDamageEvent DamageEvent;
			DamageEvent.HitInfo = Hit;
			DamageEvent.ShotDirection = OwnerCharacter->GetActorForwardVector();
			DamageEvent.DamageTypeClass = UDamageType::StaticClass();
			DamageEvent.KnockBackPower = CurrentSkillData->KnockbackPower;

			HitActor->TakeDamage(
				CurrentSkillData->AttackPower,
				DamageEvent,
				OwnerCharacter->GetController(),
				OwnerCharacter
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

void USkillComponent::CanNextCombo()
{
	if (StateComponent)
	{
		StateComponent->SetIsInAction(false);
	}
}

void USkillComponent::ResetCombo()
{
	ComboCount = 0;
}
