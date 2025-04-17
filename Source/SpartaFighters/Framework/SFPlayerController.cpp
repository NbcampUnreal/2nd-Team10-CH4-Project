#include "SFPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "Framework/SFGameInstanceSubsystem.h"
#include "Framework/SFBattleGameMode.h"
#include "Framework/SFPlayerState.h"
#include "UI/UIManager/UIManager.h"
#include "Kismet/GameplayStatics.h"

#include "Character/SFCharacter.h"
#include "Inventory/SFInventoryComponent.h"
#include "Items/SFItemBase.h"
#include "Inventory/SFInventoryInteractionComponent.h"

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
	SetupCharacterInventory();
	if (IsLocalController())
	{
		AddMappingContext();
		SetInputMode(FInputModeGameOnly());
	}
}

void ASFPlayerController::SetupCharacterInventory()
{
	ASFCharacter* PossessedCharacter = Cast<ASFCharacter>(GetPawn());
	if (PossessedCharacter && GetLocalRole() == ROLE_Authority)
	{
		UE_LOG(LogTemp, Warning, TEXT("Possessed Character exist (Authority)"));
		USFInventoryComponent* InventoryComp = PossessedCharacter->FindComponentByClass<USFInventoryComponent>();
		USFInventoryInteractionComponent* InteractionComp = PossessedCharacter->FindComponentByClass<USFInventoryInteractionComponent>();

		if (InventoryComp && InteractionComp)
		{
			UE_LOG(LogTemp, Warning, TEXT("InventoryComponent and InventoryInteractionComponent exist"));
			if (ASFPlayerState* PS = GetPlayerState<ASFPlayerState>())
			{
				UE_LOG(LogTemp, Warning, TEXT("PlayerState unique ID: %s"), *PS->GetUniqueID());
				// ServerRPC 
				InteractionComp->Server_GetPendingPurchases(PS->GetUniqueID());
			}
		}
	}
}

//Inventory logic for shop
//void ASFPlayerController::SetupCharacterInventory() 
//{
//	ASFCharacter* PossessedCharacter = Cast<ASFCharacter>(GetPawn());
//	if (PossessedCharacter)
//	{
//		UE_LOG(LogTemp, Warning, TEXT("Possessed Character exist"));
//		USFInventoryComponent* InventoryComp = PossessedCharacter->FindComponentByClass<USFInventoryComponent>();
//		USFGameInstanceSubsystem* GameInstanceSubsystem = GetGameInstanceSubsystem();
//
//		if (InventoryComp && GameInstanceSubsystem)
//		{
//			UE_LOG(LogTemp, Warning, TEXT("InventoryComponent and GameInstanceSubsystem exist"));
//			if (ASFPlayerState* PS = GetPlayerState<ASFPlayerState>())
//			{
//				UE_LOG(LogTemp, Warning, TEXT("PlayerState unique ID: %s"), *PS->GetUniqueID());
//				TArray<TSubclassOf<class USFItemBase>> PendingPurchases = GameInstanceSubsystem->GetPendingShopPurchases(PS->GetUniqueID());
//				UE_LOG(LogTemp, Warning, TEXT("Pending Purchases Count: %d"), PendingPurchases.Num());
//				for (auto& ItemClass : PendingPurchases)
//				{
//					UE_LOG(LogTemp, Warning, TEXT("Attempting to add item class: %s"), *ItemClass->GetName());
//					InventoryComp->Server_AddItemByClass(ItemClass);
//					UE_LOG(LogTemp, Warning, TEXT("Item added"));
//				}
//				GameInstanceSubsystem->ClearPendingShopPurchases(PS->GetUniqueID());
//				UE_LOG(LogTemp, Warning, TEXT("Item well added and shop purchase cleared"));
//			}
//		}
//	}
//}

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