#include "Character/SFCharacter.h"
#include "Framework/SFPlayerController.h"
#include "EnhancedInputComponent.h"
#include "Components/MovementInputComponent.h"
#include "Components/StatusContainerComponent.h"

ASFCharacter::ASFCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	MovementInputComponent = CreateDefaultSubobject<UMovementInputComponent>(TEXT("MoveInputComponent"));
	StatusContainerComponent = CreateDefaultSubobject<UStatusContainerComponent>(TEXT("StatusContainerComponent"));
}

UStatusContainerComponent* ASFCharacter::GetStatusContainerComponent() const
{
	return StatusContainerComponent;
}

void ASFCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	InitializeCharacterProperties();
}

void ASFCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ASFCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (ASFPlayerController* SFPlayerController = Cast<ASFPlayerController>(GetController()))
	{
		if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
		{
			MovementInputComponent->SetupInput(EnhancedInput, SFPlayerController, this);
			// Other SetupInput() gogogo
		}
	}
}

void ASFCharacter::InitializeCharacterProperties()
{
	if (StatusContainerComponent)
	{
		StatusContainerComponent->InitializeMovementProperties(this);
	}

	// later add Example
	// ex) CombatComponent->InitializeCombatProperties();
}

void ASFCharacter::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);

	// Needs Reset
	if (MovementInputComponent)
	{
		MovementInputComponent->ResetJumpCount();
	}
}
