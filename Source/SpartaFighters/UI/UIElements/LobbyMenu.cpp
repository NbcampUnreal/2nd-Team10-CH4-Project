#include "LobbyMenu.h"
#include "Framework/SFGameInstance.h"
#include "Framework/SFGameInstanceSubsystem.h"
#include "ShopMenu.h"
#include "Components/Button.h"

#include "UI/UIObject/PlayerSimpleInfoWidget.h"
//#include "UI/UIObject/GlobalChatWidget.h"
#include "UI/PopUp/QuitGameWidget.h"
#include "UI/UIManager/UIManager.h"

#include "Engine/AssetManager.h"
#include "Engine/StreamableManager.h"

void ULobbyMenu::NativeConstruct()
{
	Super::NativeConstruct();

	if (PlayerInfoButton)
	{
		PlayerInfoButton->OnClicked.AddUniqueDynamic(this, &ULobbyMenu::OnPlayerInfoClicked);
	}
	if (ShopButton)
	{
		ShopButton->OnClicked.AddUniqueDynamic(this, &ULobbyMenu::OnShopClicked);
	}
	if (OptionButton)
	{
		OptionButton->OnClicked.AddUniqueDynamic(this, &ULobbyMenu::OnOptionClicked);
	}
	if (QuitGameButton)
	{
		QuitGameButton->OnClicked.AddUniqueDynamic(this, &ULobbyMenu::OnQuitGameClicked);
	}

	if (SingleGameModeButton)
	{
		SingleGameModeButton->OnClicked.AddUniqueDynamic(this, &ULobbyMenu::OnSingleGameModeClicked);
	}
	if (CoopGameModeButton)
	{
		CoopGameModeButton->OnClicked.AddUniqueDynamic(this, &ULobbyMenu::OnCoopGameModeClicked);
	}
	if (BattleGameModeButton)
	{
		BattleGameModeButton->OnClicked.AddUniqueDynamic(this, &ULobbyMenu::OnBattleGameModeClicked);
	}

	if (QuitGameWidgetClass.IsNull())
	{
		UE_LOG(LogTemp, Error, TEXT("QuitGameWidgetClass is NULL in NativeConstruct"));
	}
}

void ULobbyMenu::NativeDestruct()
{
	Super::NativeDestruct();

	if (PlayerInfoButton)
	{
		PlayerInfoButton->OnClicked.RemoveDynamic(this, &ULobbyMenu::OnPlayerInfoClicked);
	}
	if (ShopButton)
	{
		ShopButton->OnClicked.RemoveDynamic(this, &ULobbyMenu::OnShopClicked);
	}
	if (OptionButton)
	{
		OptionButton->OnClicked.RemoveDynamic(this, &ULobbyMenu::OnOptionClicked);
	}
	if (QuitGameButton)
	{
		QuitGameButton->OnClicked.RemoveDynamic(this, &ULobbyMenu::OnQuitGameClicked);
	}

	if (SingleGameModeButton)
	{
		SingleGameModeButton->OnClicked.RemoveDynamic(this, &ULobbyMenu::OnSingleGameModeClicked);
	}
	if (CoopGameModeButton)
	{
		CoopGameModeButton->OnClicked.RemoveDynamic(this, &ULobbyMenu::OnCoopGameModeClicked);
	}
	if (BattleGameModeButton)
	{
		BattleGameModeButton->OnClicked.RemoveDynamic(this, &ULobbyMenu::OnBattleGameModeClicked);
	}
}

void ULobbyMenu::OnPlayerInfoClicked()
{
	UE_LOG(LogTemp, Warning, TEXT("OnPlayerInfoClicked!"));
	// Open Player Info UI
}

void ULobbyMenu::OnShopClicked()
{
	UE_LOG(LogTemp, Warning, TEXT("OnShopClicked!"));
	if (UUIManager* UIManager = ResolveUIManager())
	{
		UIManager->ShowShopMenu();
	}
}

void ULobbyMenu::OnOptionClicked()
{
	UE_LOG(LogTemp, Warning, TEXT("OnOptionClicked!"));
	// Open the Option menu UI
}

void ULobbyMenu::OnQuitGameClicked()
{
	UE_LOG(LogTemp, Log, TEXT("Quit Game Button Clicked"));

	UClass* WidgetClass = QuitGameWidgetClass.LoadSynchronous();

	if (QuitGameWidgetClass.IsValid() == false)
	{
		UE_LOG(LogTemp, Warning, TEXT("QuitGameWidgetClass is not valid"));
		return;
	}

	if (!WidgetClass)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to load QuitGameWidgetClass"));
		return;
	}

	USelectPopUpBase* QuitGameWidgetInstance = CreateWidget<USelectPopUpBase>(GetWorld(), WidgetClass);
	if (QuitGameWidgetInstance)
	{
		QuitGameWidgetInstance->AddToViewport();
		UE_LOG(LogTemp, Log, TEXT("Quit Game Widget Added to Viewport"));
	}
}

void ULobbyMenu::OnSingleGameModeClicked()
{
	UE_LOG(LogTemp, Warning, TEXT("OnSingleGameModeClicked!"));

	if (USFGameInstance* GameInstance = Cast<USFGameInstance>(GetGameInstance()))
	{
		if (USFGameInstanceSubsystem* Subsystem = GameInstance->GetSubsystem<USFGameInstanceSubsystem>())
		{
			Subsystem->SetCurrentGameMode(EGameModeType::Single);

			if (UUIManager* UIManager = ResolveUIManager())
			{
				UIManager->ShowMapSelectionWidget(Subsystem->GetCurrentGameMode());
			}
		}
	}
}

void ULobbyMenu::OnCoopGameModeClicked()
{
	UE_LOG(LogTemp, Warning, TEXT("OnCoopGameModeClicked!"));

	if (USFGameInstance* GameInstance = Cast<USFGameInstance>(GetGameInstance()))
	{
		if (USFGameInstanceSubsystem* Subsystem = GameInstance->GetSubsystem<USFGameInstanceSubsystem>())
		{
			Subsystem->SetCurrentGameMode(EGameModeType::Cooperative);

			const FString HostAddress = TEXT("127.0.0.1:17777");
			Subsystem->ConnectToServerByAddress(HostAddress);

			/*const FString RoomMapName = TEXT("RoomMenu");
			Subsystem->ChangeLevelByMapName(RoomMapName);*/
		}
	}
}

void ULobbyMenu::OnBattleGameModeClicked()
{
	UE_LOG(LogTemp, Warning, TEXT("OnBattleGameModeClicked!"));

	if (USFGameInstance* GameInstance = Cast<USFGameInstance>(GetGameInstance()))
	{
		if (USFGameInstanceSubsystem* Subsystem = GameInstance->GetSubsystem<USFGameInstanceSubsystem>())
		{
			Subsystem->SetCurrentGameMode(EGameModeType::Battle);

			const FString HostAddress = TEXT("127.0.0.1:17777");
			Subsystem->ConnectToServerByAddress(HostAddress);

			/*const FString RoomMapName = TEXT("RoomMenu");
			Subsystem->ChangeLevelByMapName(RoomMapName);*/
		}
	}
}