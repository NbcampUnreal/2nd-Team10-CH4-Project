#include "Character/Components/StatusContainerComponent.h"
#include "Character/SFCharacter.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/CharacterMovementComponent.h"

UStatusContainerComponent::UStatusContainerComponent()
{
	SetIsReplicatedByDefault(true);
	bWantsInitializeComponent = true;

}

void UStatusContainerComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UStatusContainerComponent, CurrentHP);
	DOREPLIFETIME(UStatusContainerComponent, MaxHP);
}

void UStatusContainerComponent::BeginPlay()
{
	Super::BeginPlay();

}

//void UStatusContainerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
//{
	//Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
//}

FStatusStruct UStatusContainerComponent::GetStruct() const
{
	return StatusStruct;
}

float UStatusContainerComponent::GetStatusValue(EStatusType Type)
{
	if (StatusStruct.StatusValues.Contains(Type))
	{
		return StatusStruct.StatusValues[Type];
	}
	else
	{
		ensureAlways(TEXT("Status Type is Empty"));
		return 0.0f;
	}
}

void UStatusContainerComponent::InitStatusComponent(ACharacter* Character)
{
	InitializeMovementProperties(Character);

	MaxHP = StatusStruct.Get(EStatusType::MaxHP);
	CurrentHP = StatusStruct.Get(EStatusType::CurHP);
}

void UStatusContainerComponent::InitializeMovementProperties(ACharacter* Character)
{
	ACharacter* OwnerCharacter = Character;

	//ASFCharacter* SFCharacter = Cast<ASFCharacter>(OwnerCharacter);

	if (!OwnerCharacter || !OwnerCharacter->GetCharacterMovement()) { return; }
	//FStatusStruct CharacterStatus = SFCharacter->GetStatusContainerComponent()->GetStruct();
	UCharacterMovementComponent* Movement = OwnerCharacter->GetCharacterMovement();

	Movement->JumpZVelocity = StatusStruct.Get(EStatusType::JumpPower);
	Movement->MaxWalkSpeed = StatusStruct.Get(EStatusType::MoveSpeed);
	
	//Movement->AirControl = 1.0f;  // TODO : can be changed value
	//OwnerCharacter->JumpMaxCount = 2; // TODO : can be changed value
	//OwnerCharacter->bUseControllerRotationYaw = false;	
}

void UStatusContainerComponent::ModifyStatus(EStatusType Type, float Amount)
{
	switch (Type)
	{
	case EStatusType::CurHP:
		SetHP(Amount);
		break;
	default:
		float OriginValue = StatusStruct.Get(Type);
		float ChangedValue = OriginValue + Amount;
		StatusStruct.Set(Type, ChangedValue);
		break;
	}
}

void UStatusContainerComponent::SetHP(float Amount)
{
	CurrentHP = FMath::Clamp(CurrentHP + Amount, 0.0f, MaxHP);
	StatusStruct.Set((EStatusType::CurHP), CurrentHP);

	UE_LOG(LogTemp, Display, TEXT("CurrentHP Changed"));

	ENetMode NetMode = GetNetMode();
	if (NetMode == NM_Standalone || NetMode == NM_ListenServer)
	{
		OnRep_CurrentHP();
	}
}

void UStatusContainerComponent::OnRep_CurrentHP()
{
	UE_LOG(LogTemp, Display, TEXT("OnRep_CurrentHP"));

	// TO DO : Update UI Or Something

}