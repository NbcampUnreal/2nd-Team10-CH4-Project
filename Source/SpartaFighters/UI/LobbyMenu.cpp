#include "UI/LobbyMenu.h"
#include "Components/Button.h"
#include "Components/VerticalBox.h"
#include "UI/UIObject/PlayerSimpleInfoWidget.h"
#include "UI/UIObject/GlobalChatWidget.h"
#include "UI/UIObject/RoomListWidget.h"
#include "UI/QuitGameWidget.h"
#include "UI/PopUp/CreateRoomWidget.h"
#include "Engine/AssetManager.h"
#include "Engine/StreamableManager.h"

void ULobbyMenu::NativeConstruct()
{
	Super::NativeConstruct();

	if (PlayerInfoButton)
	{
		PlayerInfoButton->OnClicked.AddDynamic(this, &ULobbyMenu::OnPlayerInfoClicked);
	}
	if (ShopButton)
	{
		ShopButton->OnClicked.AddDynamic(this, &ULobbyMenu::OnShopClicked);
	}
	if (CreateRoomButton)
	{
		CreateRoomButton->OnClicked.AddDynamic(this, &ULobbyMenu::OnCreateRoomClicked);
	}
	if (OptionButton)
	{
		OptionButton->OnClicked.AddDynamic(this, &ULobbyMenu::OnOptionClicked);
	}
	if (QuitGameButton)
	{
		QuitGameButton->OnClicked.AddDynamic(this, &ULobbyMenu::OnQuitGameClicked);
	}
	if (CooperativeFilterButton)
	{
		CooperativeFilterButton->OnClicked.AddDynamic(this, &ULobbyMenu::OnCooperativeFilterClicked);
	}
	if (BattleFilterButton)
	{
		BattleFilterButton->OnClicked.AddDynamic(this, &ULobbyMenu::OnBattleFilterClicked);
	}

	if (QuitGameWidgetClass.IsNull())
	{
		UE_LOG(LogTemp, Error, TEXT("QuitGameWidgetClass is NULL in NativeConstruct"));
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
	// Open the Shop menu UI
}

void ULobbyMenu::OnCreateRoomClicked()
{
	UE_LOG(LogTemp, Warning, TEXT("OnCreateRoomClicked!"));

	if (CachedCreateRoomWidget && CachedCreateRoomWidget->IsValidLowLevel())
	{
		UE_LOG(LogTemp, Warning, TEXT("CachedCreateRoomWidget->IsValidLowLevel()!"));
		CachedCreateRoomWidget->ResetRoomNameText();
		CachedCreateRoomWidget->SetVisibility(ESlateVisibility::Visible);
		return;
	}

	UClass* WidgetClass = CreateRoomWidgetClass.LoadSynchronous();

	if (CreateRoomWidgetClass.IsValid() == false)
	{
		UE_LOG(LogTemp, Warning, TEXT("CreateRoomWidgetClass is not valid"));
		return;
	}

	if (!WidgetClass)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to load QuitGameWidgetClass"));
		return;
	}

	CachedCreateRoomWidget = CreateWidget<UCreateRoomWidget>(GetWorld(), WidgetClass);
	if (CachedCreateRoomWidget)
	{
		UE_LOG(LogTemp, Warning, TEXT("OnCreateRoomClicked!"));
		CachedCreateRoomWidget->ResetRoomNameText();
		CachedCreateRoomWidget->AddToViewport();
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

void ULobbyMenu::OnCooperativeFilterClicked()
{
	UE_LOG(LogTemp, Warning, TEXT("OnCooperativeFilterClicked!"));
	// Filter the list of Cooperative rooms
}

void ULobbyMenu::OnBattleFilterClicked()
{
	UE_LOG(LogTemp, Warning, TEXT("OnBattleFilterClicked!"));
	// Filter the list of Battle rooms
}
