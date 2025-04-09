#include "UIManager.h"
#include "UIManagerSettings.h"

#include "UI/UIElements/LoginMenu.h"
#include "UI/UIElements/LobbyMenu.h"
#include "UI/UIElements/ShopMenu.h"
#include "UI/UIElements/LoginMenu.h"
#include "UI/UIElements/ShopItemListMenu.h"
#include "UI/UIElements/RoomWidget.h"
#include "UI/UIObject/MapSelectionWidget.h"

#include "Blueprint/WidgetBlueprintLibrary.h"

#include "Kismet/GameplayStatics.h"
#include "Framework/SFGameInstance.h"

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

			if (LoginMenuClass)
			{
				CachedLoginMenu = CreateWidget<ULoginMenu>(OwningPlayer, LoginMenuClass);
			}

			if (LobbyMenuClass)
			{
				CachedLobbyMenu = CreateWidget<ULobbyMenu>(OwningPlayer, LobbyMenuClass);
				CachedLobbyMenu->SetVisibility(ESlateVisibility::Hidden);
			}

			if (LobbyMenuClass)
			{
				CachedRoomMenu = CreateWidget<URoomWidget>(OwningPlayer, RoomWidgetClass);
				CachedRoomMenu->SetVisibility(ESlateVisibility::Hidden);
			}

			if (ShopMenuClass)
			{
				CachedShopMenu = CreateWidget<UShopMenu>(OwningPlayer, ShopMenuClass);
				CachedShopMenu->SetVisibility(ESlateVisibility::Hidden);
			}

			if (ShopItemListMenuClass)
			{
				CachedShopItemListMenu = CreateWidget<UShopItemListMenu>(OwningPlayer, ShopItemListMenuClass);
				CachedShopItemListMenu->AddToViewport();
				CachedShopItemListMenu->SetVisibility(ESlateVisibility::Hidden);
			}
		}
	}
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
	if (!NewWidget || IsRunningDedicatedServer()) return;

	if (CurrentWidget && CurrentWidget != NewWidget)
	{
		CurrentWidget->RemoveFromViewport();
	}

	NewWidget->AddToViewport();
	NewWidget->SetVisibility(ESlateVisibility::Visible);
	CurrentWidget = NewWidget;

	if (OwningPlayer)
	{
		FInputModeUIOnly InputMode;
		InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
		OwningPlayer->SetInputMode(InputMode);
		OwningPlayer->bShowMouseCursor = true;
	}
}


void UUIManager::BackToLobbyMenu()
{
	CachedShopMenu->SetVisibility(ESlateVisibility::Hidden);
	CachedShopItemListMenu->SetVisibility(ESlateVisibility::Hidden);
	CachedLobbyMenu->SetVisibility(ESlateVisibility::Visible);
}

void UUIManager::ShowCreatedRoomUI(const FRoomInfo& RoomInfo)
{
	/*URoomWidget* RoomWidgetInstance = CreateWidget<URoomWidget>(GetWorld(), RoomWidgetClass);
	if (RoomWidgetInstance)
	{
		RoomWidgetInstance->SetupRoom(RoomInfo);

		TArray<FPlayerInfo> NewPlayerList;
		FPlayerInfo Player;
		Player.PlayerID = RoomInfo.OwnerPlayerID;
		Player.bIsReady = false;
		NewPlayerList.Add(Player);
		RoomWidgetInstance->SetPlayerList(NewPlayerList);

		if (RoomWidgetInstance->MapSelectionWidgetClass)
		{
			RoomWidgetInstance->MapSelectionWidgetClass->SetGameMode(RoomInfo.GameMode);
		}

		RoomWidgetInstance->AddToViewport();

		TArray<UUserWidget*> FoundWidgets;
		UWidgetBlueprintLibrary::GetAllWidgetsOfClass(GetWorld(), FoundWidgets, ULobbyMenu::StaticClass(), false);
		for (UUserWidget* Widget : FoundWidgets)
		{
			if (ULobbyMenu* LobbyMenu = Cast<ULobbyMenu>(Widget))
			{
				LobbyMenu->SetVisibility(ESlateVisibility::Hidden);
				break;
			}
		}
	}*/
}

void UUIManager::OpenRoomWidget(const FRoomInfo& RoomInfo)
{
	/*UE_LOG(LogTemp, Warning, TEXT("OpenRoomWidget Called!RoomID: % d"), RoomInfo.RoomID);

	URoomWidget* RoomWidgetInstance = CreateWidget<URoomWidget>(GetWorld(), RoomWidgetClass);
	if (RoomWidgetInstance)
	{
		RoomWidgetInstance->SetPlayerList(RoomInfo.PlayerList.Items);
		RoomWidgetInstance->SetupRoom(RoomInfo);
		if (RoomWidgetInstance->MapSelectionWidgetClass)
		{
			RoomWidgetInstance->MapSelectionWidgetClass->SetIsEnabled(false);
		}

		RoomWidgetInstance->AddToViewport();

		TArray<UUserWidget*> FoundWidgets;
		UWidgetBlueprintLibrary::GetAllWidgetsOfClass(GetWorld(), FoundWidgets, ULobbyMenu::StaticClass(), false);
		for (UUserWidget* Widget : FoundWidgets)
		{
			if (ULobbyMenu* LobbyMenu = Cast<ULobbyMenu>(Widget))
			{
				LobbyMenu->SetVisibility(ESlateVisibility::Hidden);
				break;
			}
		}
	}*/
}
