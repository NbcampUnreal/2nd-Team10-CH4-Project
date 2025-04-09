#include "SFPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "Framework/SFGameInstanceSubsystem.h"
#include "UI/UIManager/UIManager.h"

ASFPlayerController::ASFPlayerController()
{
	InputMappingContext = nullptr;

	MoveAction = nullptr;
	JumpAction = nullptr;
	RollAction = nullptr;
	AttackAction = nullptr;
	SkillAttackAction = nullptr;
	GuardAction = nullptr;
	CrouchAction = nullptr;
}

void ASFPlayerController::BeginPlay()
{
	Super::BeginPlay();

	AddMappingContext();
	InitUI();
}

void ASFPlayerController::AddMappingContext()
{
	ULocalPlayer* LocalPlayer = GetLocalPlayer();
	UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	if (!Subsystem)
	{
		return;
	}

	if (!InputMappingContext)
	{
		return;
	}

	Subsystem->AddMappingContext(InputMappingContext, 0);
}

void ASFPlayerController::InitUI()
{
	if (USFGameInstanceSubsystem* Subsystem = GetGameInstance()->GetSubsystem<USFGameInstanceSubsystem>())
	{
		if (UUIManager* UIManager = Subsystem->GetUIManager())
		{
			// DataAsset도 같이 넘겨주자
			UIManager->Init(this);
		}
	}
}