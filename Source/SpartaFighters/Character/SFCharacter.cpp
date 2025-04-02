#include "Character/SFCharacter.h"
#include "Framework/SFPlayerController.h"
#include "EnhancedInputComponent.h"
#include "Components/MovementInputComponent.h"

ASFCharacter::ASFCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	MovementInputComponent = CreateDefaultSubobject<UMovementInputComponent>(TEXT("MoveInputComponent"));
}

void ASFCharacter::BeginPlay()
{
	Super::BeginPlay();
	
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

void ASFCharacter::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);

	// ���⼭ InputComponent�� �����ؼ� ���� ī��Ʈ �ʱ�ȭ����� ��
	if (MovementInputComponent)
	{
		MovementInputComponent->ResetJumpCount();
	}
}
