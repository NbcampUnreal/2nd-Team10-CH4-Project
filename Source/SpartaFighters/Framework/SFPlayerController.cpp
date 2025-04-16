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
	SetupCharacterInventory();
	if (IsLocalController())
	{
		AddMappingContext();
		SetInputMode(FInputModeGameOnly());
	}
}

//Inventory logic for shop
void ASFPlayerController::SetupCharacterInventory() 
{
	ASFCharacter* PossessedCharacter = Cast<ASFCharacter>(GetPawn());
	if (PossessedCharacter)
	{
		UE_LOG(LogTemp, Warning, TEXT("Possessed Character exist"));
		USFInventoryComponent* InventoryComp = PossessedCharacter->FindComponentByClass<USFInventoryComponent>();
		USFGameInstanceSubsystem* GameInstanceSubsystem = GetGameInstanceSubsystem();

		if (InventoryComp && GameInstanceSubsystem)
		{
			UE_LOG(LogTemp, Warning, TEXT("InventoryComponent and GameInstanceSubsystem exist"));
			if (ASFPlayerState* PS = GetPlayerState<ASFPlayerState>())
			{
				UE_LOG(LogTemp, Warning, TEXT("PlayerState unique ID: %s"), *PS->GetUniqueID());
				TArray<TSubclassOf<class USFItemBase>> PendingPurchases = GameInstanceSubsystem->GetPendingShopPurchases(PS->GetUniqueID());
				for (auto& ItemClass : PendingPurchases)
				{
					InventoryComp->Server_AddItemByClass(ItemClass);
					UE_LOG(LogTemp, Warning, TEXT("Item added"));
				}
				GameInstanceSubsystem->ClearPendingShopPurchases(PS->GetUniqueID());
				UE_LOG(LogTemp, Warning, TEXT("Item well added and shop purchase cleared"));
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