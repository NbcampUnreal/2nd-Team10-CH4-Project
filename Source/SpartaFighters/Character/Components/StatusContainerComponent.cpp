#include "Character/Components/StatusContainerComponent.h"
#include "Character/SFCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

UStatusContainerComponent::UStatusContainerComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void UStatusContainerComponent::BeginPlay()
{
	Super::BeginPlay();

}

FStatusStruct UStatusContainerComponent::GetStruct() const
{
	return StatusStruct;
}

void UStatusContainerComponent::InitializeMovementProperties(ASFCharacter* SFCharacter)
{
	ASFCharacter* OwnerCharacter = SFCharacter;

	if (!OwnerCharacter || !OwnerCharacter->GetCharacterMovement()) { return; }

	FStatusStruct StatStruct = OwnerCharacter->GetStatusContainerComponent()->GetStruct();
	UCharacterMovementComponent* Movement = OwnerCharacter->GetCharacterMovement();

	Movement->JumpZVelocity = StatStruct.Get(EStatusType::JumpPower);
	Movement->MaxWalkSpeed = StatStruct.Get(EStatusType::MoveSpeed);
	Movement->AirControl = 1.0f;  // TODO : can be changed value
	OwnerCharacter->JumpMaxCount = 2; // TODO : can be changed value
	OwnerCharacter->bUseControllerRotationYaw = false;
}

// Temporary for test
void UStatusContainerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Test : Decrease HP for 1s
	float CurHP = StatusStruct.Get(EStatusType::CurHP);
	StatusStruct.Set(EStatusType::CurHP, FMath::Max(CurHP - 1.f * DeltaTime, 0.f));

	// Print HP
	//UE_LOG(LogTemp, Log, TEXT("Tick HP: %.1f"), StatusStruct.Get(EStatusType::CurHP));
}
