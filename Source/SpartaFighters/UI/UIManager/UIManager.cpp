#include "UIManager.h"
#include "UIManagerSettings.h"

#include "UI/UIElements/LoginMenu.h"
#include "UI/UIElements/LobbyMenu.h"
#include "UI/UIElements/ShopMenu.h"
#include "UI/UIElements/ShopItemListMenu.h"
#include "UI/UIElements/RoomWidget.h"
#include "UI/UIElements/CombatHUD.h"
#include "UI/UIElements/CombatResultHUD.h"
#include "UI/UIObject/MapSelectionWidget.h"
#include "UI/UIObject/SelectCharacterWidget.h"
#include "UI/UIObject/OptionsWidget.h"
#include "Components/TextBlock.h"

#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Framework/SFGameInstance.h"
#include "Framework/SFGameStateBase.h"

#include "Kismet/GameplayStatics.h"

UUIManager::UUIManager()
{
	CurrentWidget = nullptr;
}

void UUIManager::Init(APlayerController* PlayerController)
{
	OwningPlayer = PlayerController;

	if (const USFGameInstance* GameInstance = Cast<USFGameInstance>(PlayerController->GetGameInstance()))
	{
		if (const UUIManagerSettings* Settings = GameInstance->GetUISettings())
		{
			LoginMenuClass = Settings->FromBPLoginMenuClass;
			LobbyMenuClass = Settings->FromBPLobbyMenuClass;
			ShopMenuClass = Settings->FromBPShopMenuClass;
			ShopItemListMenuClass = Settings->FromBPShopItemListMenuClass;
			RoomWidgetClass = Settings->FromBPRoomWidgetClass;
			MapSelectionWidgetClass = Settings->FromBPMapSelectionWidgetClass;
			CombatHUDClass = Settings->FromBPCombatHUDClass;
			CombatResultHUDClass = Settings->FromBPCombatResultHUDClass;
			SelectCharacterWidgetClass = Settings->FromBPSelectCharacterWidgetClass;
			OptionsWidgetClass = Settings->FromBPOptionsWidgetClass;

			if (!CachedLoginMenu && LoginMenuClass)
			{
				CachedLoginMenu = CreateWidget<ULoginMenu>(OwningPlayer, LoginMenuClass);
			}
			if (!CachedLobbyMenu && LobbyMenuClass)
			{
				CachedLobbyMenu = CreateWidget<ULobbyMenu>(OwningPlayer, LobbyMenuClass);
			}
			if (!CachedRoomMenu && RoomWidgetClass)
			{
				CachedRoomMenu = CreateWidget<URoomWidget>(OwningPlayer, RoomWidgetClass);
			}
			if (!CachedShopMenu && ShopMenuClass)
			{
				CachedShopMenu = CreateWidget<UShopMenu>(OwningPlayer, ShopMenuClass);
			}
			if (!CachedShopItemListMenu && ShopItemListMenuClass)
			{
				CachedShopItemListMenu = CreateWidget<UShopItemListMenu>(OwningPlayer, ShopItemListMenuClass);
			}
			if (!CachedCombatHUD && CombatHUDClass)
			{
				CachedCombatHUD = CreateWidget<UCombatHUD>(OwningPlayer, CombatHUDClass);
				//CachedCombatHUD->AddToViewport();
			}
			if (!CachedCombatResultHUD && CombatResultHUDClass)
			{
				CachedCombatResultHUD = CreateWidget<UCombatResultHUD>(OwningPlayer, CombatResultHUDClass);
				//CachedCombatResultHUD->AddToViewport();
			}
			if (!CachedOptionsWidget && OptionsWidgetClass)
			{
				CachedOptionsWidget = CreateWidget<UOptionsWidget>(OwningPlayer, OptionsWidgetClass);
				CachedOptionsWidget->AddToViewport(100);
			}
		}
	}
}

void UUIManager::BeginDestroy()
{
	Super::BeginDestroy();
}

void UUIManager::ShowLoginMenu()
{
	UE_LOG(LogTemp, Warning, TEXT("ShowLoginMenu"));
	SwitchToWidget(CachedLoginMenu);
}

void UUIManager::ShowLobbyMenu()
{
	UE_LOG(LogTemp, Warning, TEXT("ShowLobbyMenu"));
	SwitchToWidget(CachedLobbyMenu);
}

void UUIManager::ShowRoomMenu()
{
	UE_LOG(LogTemp, Warning, TEXT("ShowRoomMenu"));
	SwitchToWidget(CachedRoomMenu);
}

void UUIManager::ShowShopMenu()
{
	UE_LOG(LogTemp, Warning, TEXT("ShowShopMenu"));
	SwitchToWidget(CachedShopMenu);
}

void UUIManager::ShowShopItemListMenu()
{
	UE_LOG(LogTemp, Warning, TEXT("ShowShopItemListMenu"));
	SwitchToWidget(CachedShopItemListMenu);
}

void UUIManager::SwitchToWidget(UUserWidget* NewWidget)
{
	if (!NewWidget || IsRunningDedicatedServer())
	{
		return;
	}
	
	if (CurrentWidget && CurrentWidget != NewWidget)
	{
		if (CurrentWidget->IsInViewport())
		{
			CurrentWidget->RemoveFromParent();
		}
	}

	if (!NewWidget->IsInViewport())
	{
		NewWidget->AddToViewport();
	}
	else
	{
		NewWidget->SetVisibility(ESlateVisibility::Visible);
	}

	CurrentWidget = NewWidget;

	if (OwningPlayer)
	{
		FInputModeUIOnly InputMode;
		InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
		OwningPlayer->SetInputMode(InputMode);
		OwningPlayer->bShowMouseCursor = true;
	}
}

void UUIManager::ShowMapSelectionWidget(EGameModeType GameModeType)
{
	if (!MapSelectionWidgetInstance && MapSelectionWidgetClass)
	{
		MapSelectionWidgetInstance = CreateWidget<UMapSelectionWidget>(GetWorld(), MapSelectionWidgetClass);
		MapSelectionWidgetInstance->AddToViewport();
	}

	if (MapSelectionWidgetInstance)
	{
		MapSelectionWidgetInstance->SetGameMode(GameModeType);
		MapSelectionWidgetInstance->SetVisibility(ESlateVisibility::Visible);
	}
}

void UUIManager::ShowSelectCharacterWidget()
{
	if (!SelectCharacterWidgetInstance && SelectCharacterWidgetClass)
	{
		SelectCharacterWidgetInstance = CreateWidget<USelectCharacterWidget>(GetWorld(), SelectCharacterWidgetClass);
		SelectCharacterWidgetInstance->AddToViewport();
	}

	if (SelectCharacterWidgetInstance)
	{
		SelectCharacterWidgetInstance->SetVisibility(ESlateVisibility::Visible);
	}
}

void UUIManager::ShowCombatHUD()
{
	//ensureAlways(CachedCombatHUD);
	//CachedCombatHUD->AddToViewport();
	//UE_LOG(LogTemp, Warning, TEXT("Show Combat HUD Completed!!"));

	if (CachedCombatHUD)
	{
		CachedCombatHUD->AddToViewport();
	}
	else
	{
		// Handle the error, log it, or initialize the HUD properly
	}
}

void UUIManager::ShowOptionsWidget()
{
	ensureAlways(CachedOptionsWidget);
	CachedOptionsWidget->AddToViewport();
	CachedOptionsWidget->SetVisibility(ESlateVisibility::Visible);
}

void UUIManager::CloseOptionsWidget()
{
	ensureAlways(CachedOptionsWidget);
	CachedOptionsWidget->SetVisibility(ESlateVisibility::Hidden);
}

void UUIManager::ShowCombatResultHUD()
{
	ensureAlways(CachedCombatResultHUD);
	CachedCombatResultHUD->AddToViewport();
}

void UUIManager::CloseCombatHUD()
{
	ensureAlways(CachedCombatHUD);
	CachedCombatHUD->SetVisibility(ESlateVisibility::Hidden);
}

void UUIManager::CloseCombatResultHUD()
{
	ensureAlways(CachedCombatResultHUD);
	CachedCombatResultHUD->SetVisibility(ESlateVisibility::Hidden);
}

void UUIManager::UpdateCombatHUD()
{
	if (UTextBlock* PlayTimeText = Cast<UTextBlock>(CachedCombatHUD->GetWidgetFromName(TEXT("PlayTimerTextBlock"))))
	{
		float RemainingTime = 0.f;

		if (ASFGameStateBase* GameState = OwningPlayer->GetWorld()->GetGameState<ASFGameStateBase>())
		{
			RemainingTime = GameState->GetRemainingBattleTime();
		}

		int32 Minutes = FMath::FloorToInt(RemainingTime / 60.f);
		int32 Seconds = FMath::FloorToInt(FMath::Fmod(RemainingTime, 60.f));

		PlayTimeText->SetText(FText::FromString(FString::Printf(TEXT("%d : %02d"), Minutes, Seconds)));
	}
}

void UUIManager::UpdateHUD()
{
	UpdateCombatHUD();
}

void UUIManager::StartHUDUpdate()
{
	if (UWorld* World = OwningPlayer ? OwningPlayer->GetWorld() : nullptr)
	{
		World->GetTimerManager().SetTimer(
			HUDUpdateTimerHandle,
			this,
			&UUIManager::UpdateHUD,
			0.2f,
			true
		);
	}
}

void UUIManager::SetPlayerController(APlayerController* PlayerController)
{
	OwningPlayer = PlayerController;
}