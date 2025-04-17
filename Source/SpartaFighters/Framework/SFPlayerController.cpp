#include "SFPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "Framework/SFGameInstanceSubsystem.h"
#include "Framework/SFBattleGameMode.h"
#include "Framework/SFPlayerState.h"
#include "UI/UIManager/UIManager.h"
#include "Kismet/GameplayStatics.h"

#include "Character/SFCharacter.h"
#include "Inventory/SFInventoryComponent.h"

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
			UIManager->DelayStartCombatResultUpdate(); // delay for 1s.. Becuase UX Up!!
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
		SetupCharacterInventory();
	}

	//if (GetLocalRole() == ROLE_Authority)
	//{
	//	SetupCharacterInventory();
	//}
}

//Inventory logic for shop
void ASFPlayerController::SetupCharacterInventory() 
{
	ASFCharacter* PossessedCharacter = Cast<ASFCharacter>(GetPawn());
	if (PossessedCharacter)
	{
		USFInventoryComponent* InventoryComp = PossessedCharacter->FindComponentByClass<USFInventoryComponent>();
		USFGameInstanceSubsystem* GameInstanceSubsystem = GetGameInstanceSubsystem();

		if (InventoryComp && GameInstanceSubsystem)
		{
			if (ASFPlayerState* PS = GetPlayerState<ASFPlayerState>())
			{
				TArray<TSubclassOf<class USFItemBase>> PendingPurchases = GameInstanceSubsystem->GetPendingShopPurchases(PS->GetUniqueID());
				for (const auto& ItemClass : PendingPurchases)
				{
					InventoryComp->Server_AddItemByClass(ItemClass);
				}
				GameInstanceSubsystem->ClearPendingShopPurchases(PS->GetUniqueID());
			}
		}
	}
}

USFGameInstanceSubsystem* ASFPlayerController::GetGameInstanceSubsystem() const
{
	UGameInstance* GameInstance = GetGameInstance();
	if (GameInstance)
	{
		return GameInstance->GetSubsystem<USFGameInstanceSubsystem>();
	}
	return nullptr;
}

void ASFPlayerController::Client_TravelToLobby_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("Client is returning to Lobby!"));

	//UGameplayStatics::OpenLevel(this, FName("/Game/SpartaFighters/Level/Menu/LobbyMenu"));
	ClientTravel("/Game/SpartaFighters/Level/Menu/LobbyMenu", ETravelType::TRAVEL_Absolute);
}