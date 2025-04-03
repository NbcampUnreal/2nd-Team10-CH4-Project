#include "Framework/SFPlayerController.h"
#include "EnhancedInputSubsystems.h"

ASFPlayerController::ASFPlayerController()
{
}

void ASFPlayerController::BeginPlay()
{
	Super::BeginPlay();

	ULocalPlayer* LocalPlayer = GetLocalPlayer();
	if (!LocalPlayer)
	{
		return;
	}

	UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	if (!Subsystem)
	{
		return;
	}

	if (!DefaultMappingContext)
	{
		return;
	}

	Subsystem->AddMappingContext(DefaultMappingContext, 0);
}
