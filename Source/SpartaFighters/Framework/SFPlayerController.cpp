#include "SFPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "Framework/SFGameInstanceSubsystem.h"
#include "Framework/SFBattleGameMode.h"
#include "Framework/SFPlayerState.h"
#include "UI/UIManager/UIManager.h"

ASFPlayerController::ASFPlayerController()
{
	InputMappingContext = nullptr;

	MoveAction = nullptr;
	JumpAction = nullptr;
	DodgeAction = nullptr;
	AttackAction = nullptr;
	SkillAttackAction = nullptr;
	GuardAction = nullptr;
	CrouchAction = nullptr;
}

void ASFPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (IsLocalController())
	{
		AddMappingContext();
		SetInputMode(FInputModeGameOnly());

		if (USFGameInstanceSubsystem* Subsystem = GetGameInstance()->GetSubsystem<USFGameInstanceSubsystem>())
		{
			if (UUIManager* UIManager = Subsystem->GetUIManager())
			{
				UIManager->SetPlayerController(this);
				UIManager->ShowCombatHUD();
			}
		}
	}
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

void ASFPlayerController::Server_RequestSpawnCharacter_Implementation()
{
	ASFBattleGameMode* GM = GetWorld()->GetAuthGameMode<ASFBattleGameMode>();
	if (GM)
	{
		GM->HandleCharacterSpawnRequest(this);
	}
}

void ASFPlayerController::Client_StartHUDUpdate_Implementation()
{
	if (USFGameInstanceSubsystem* Subsystem = GetGameInstance()->GetSubsystem<USFGameInstanceSubsystem>())
	{
		UUIManager* UIManager = Subsystem->GetUIManager();
		if (UIManager != nullptr)
		{
			UIManager->SetPlayerController(this);
			UIManager->StartHUDUpdate();
		}
	}
}

void ASFPlayerController::Client_EndBattle_Implementation()
{
	if (USFGameInstanceSubsystem* Subsystem = GetGameInstance()->GetSubsystem<USFGameInstanceSubsystem>())
	{
		UUIManager* UIManager = Subsystem->GetUIManager();
		if (UIManager != nullptr)
		{
			UIManager->EndHUDUpdate();
			UIManager->CloseCombatHUD();
			UIManager->ShowCombatResultHUD();
			UIManager->StartCombatResultUpdate();
		}
	}
}

void ASFPlayerController::Client_EndReturnToLobbyTimer_Implementation()
{
	if (USFGameInstanceSubsystem* Subsystem = GetGameInstance()->GetSubsystem<USFGameInstanceSubsystem>())
	{
		UUIManager* UIManager = Subsystem->GetUIManager();
		if (UIManager != nullptr)
		{
			UIManager->EndCombatResultUpdate();
		}
	}
}

void ASFPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	UE_LOG(LogTemp, Warning, TEXT("ASFPlayerController::OnPossess => %s"), *InPawn->GetName());

	if (IsLocalController())
	{
		AddMappingContext();
		SetInputMode(FInputModeGameOnly());
	}
}