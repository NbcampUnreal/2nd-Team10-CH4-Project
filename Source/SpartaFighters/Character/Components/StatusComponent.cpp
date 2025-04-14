#include "Character/Components/StatusComponent.h"
#include "Net/UnrealNetwork.h"

UStatusComponent::UStatusComponent()
{
	SetIsReplicatedByDefault(true);
}

void UStatusComponent::BeginPlay()
{
	Super::BeginPlay();
	InitializeStatus();
}

void UStatusComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UStatusComponent, CurHP);
	DOREPLIFETIME(UStatusComponent, MaxHP);
}

void UStatusComponent::InitializeStatus()
{
	MaxHP = StatusStruct.Get(EStatusType::MaxHP);
	CurHP = MaxHP;
	StatusStruct.Set(EStatusType::CurHP, CurHP);
}

float UStatusComponent::GetStatusValue(EStatusType Type) const
{
	return StatusStruct.Get(Type);
}

void UStatusComponent::ModifyStatus(EStatusType Type, float Amount)
{
	if (Type == EStatusType::CurHP)
	{
		ModifyHP(Amount);
		return;
	}

	float NewValue = StatusStruct.Get(Type) + Amount;
	StatusStruct.Set(Type, FMath::Max(0.f, NewValue));
}

void UStatusComponent::ModifyHP(float Amount)
{
	const float NewHP = FMath::Clamp(CurHP + Amount, 0.f, MaxHP);
	CurHP = NewHP;
	StatusStruct.Set(EStatusType::CurHP, CurHP);

	if (GetOwnerRole() == ROLE_Authority && CurHP <= 0.f)
	{
		Multicast_OnDeath();
	}
}

void UStatusComponent::OnRep_CurHP()
{
	OnHealthChanged.Broadcast(GetOwner(), CurHP);
}

void UStatusComponent::Multicast_OnDeath_Implementation()
{
	OnDeath.Broadcast();
}

//void UStatusComponent::Die()
//{
//	OnDeath.Broadcast();
//}