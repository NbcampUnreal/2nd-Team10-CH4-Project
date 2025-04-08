#include "UI/PopUp/CreateRoomWidget.h"
#include "UI/UIElements/RoomWidget.h"
#include "UI/UIElements/LobbyMenu.h"

#include "UI/UIObject/GameModeSelectionWidget.h"
#include "UI/UIObject/PlayerCountSelectionWidget.h"
#include "UI/UIObject/ItemActivationSelectionWidget.h"
#include "UI/UIObject/MapSelectionWidget.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"

#include "DataTypes/PlayerInfo.h"

#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Engine/AssetManager.h"
#include "Engine/StreamableManager.h"


FRoomInfo FRoomSettings::ToRoomInfo(int32 RoomID) const
{
	FRoomInfo RoomInfo;
	RoomInfo.RoomID = RoomID;
	RoomInfo.RoomName = RoomName;
	RoomInfo.GameMode = GameMode;
	RoomInfo.MapName = TEXT("");
	RoomInfo.CurrentPlayers = 1;
	RoomInfo.MaxPlayers = PlayerCount;
	RoomInfo.bIsGameInProgress = false;
	return RoomInfo;
}


void UCreateRoomWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (CreateRoomButton)
	{
		CreateRoomButton->OnClicked.AddDynamic(this, &UCreateRoomWidget::CreateAndOpenRoomWidget);
	}

	if (GameModeSelectionWidget.IsValid())
	{
		GameModeSelectionWidget->OnSelectionChanged.AddDynamic(this, &UCreateRoomWidget::OnGameModeChanged);
	}

	if (PlayerCountSelectionWidget.IsValid())
	{
		PlayerCountSelectionWidget->OnSelectionChanged.AddDynamic(this, &UCreateRoomWidget::OnGameModeChanged);
	}

	if (ItemActivationSelectionWidget.IsValid())
	{
		ItemActivationSelectionWidget->OnSelectionChanged.AddDynamic(this, &UCreateRoomWidget::OnGameModeChanged);
	}
}

void UCreateRoomWidget::CreateAndOpenRoomWidget()
{
	UClass* WidgetClass = RoomWidgetClass.LoadSynchronous();

	if (!RoomWidgetClass.IsValid() || !WidgetClass)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to load RoomWidgetClass"));
		return;
	}

	URoomWidget* RoomWidgetInstance = CreateWidget<URoomWidget>(GetWorld(), WidgetClass);
	if (RoomWidgetInstance)
	{
		FRoomSettings RoomSettings = GetRoomSettings();
		FRoomInfo NewRoomInfo = RoomSettings.ToRoomInfo(FMath::Rand());
		RoomWidgetInstance->SetupRoom(NewRoomInfo);

		// Create a NewPlayerList for test
		TArray<FPlayerInfo> NewPlayerList;

		FPlayerInfo Player;
		Player.PlayerID = TEXT("TestPlayer");
		Player.bIsReady = false;
		NewPlayerList.Add(Player);
		RoomWidgetInstance->SetPlayerList(NewPlayerList);
		// 
		RoomWidgetInstance->AddToViewport();

		if (RoomWidgetInstance->MapSelectionWidgetClass)
		{
			RoomWidgetInstance->MapSelectionWidgetClass->SetRoomSettings(RoomSettings);
		}
	}

	APlayerController* PlayerController = GetOwningPlayer();
	if (PlayerController)
	{
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
	}

	SetVisibility(ESlateVisibility::Hidden);
}


FString UCreateRoomWidget::GenerateRandomRoomName()
{
	static TArray<FString> Adjectives = { TEXT("Quick"), TEXT("Strong"), TEXT("Strategic"), TEXT("Mysterious") };
	static TArray<FString> Nouns = { TEXT("Combat"), TEXT("Challenges"), TEXT("Showdown"), TEXT("Arena") };

	int32 RandomAdjIndex = FMath::RandRange(0, Adjectives.Num() - 1);
	int32 RandomNounIndex = FMath::RandRange(0, Nouns.Num() - 1);

	return Adjectives[RandomAdjIndex] + TEXT(" ") + Nouns[RandomNounIndex];
}

FRoomSettings UCreateRoomWidget::GetRoomSettings() const
{
	FRoomSettings RoomSettings;

	if (!RoomNameText)
	{
		UE_LOG(LogTemp, Warning, TEXT("RoomNameText is nullptr"));
		return RoomSettings;
	}
	if (!GameModeSelectionWidget.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("GameModeSelectionWidget Is Not Valid"));
		return RoomSettings;
	}
	if (!PlayerCountSelectionWidget.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerCountSelectionWidget Is Not Valid"));
		return RoomSettings;
	}
	if (!ItemActivationSelectionWidget.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("ItemActivationSelectionWidget Is Not Valid"));
		return RoomSettings;
	}

	RoomSettings.RoomName = RoomNameText->GetText().ToString();

	UGameModeSelectionWidget* GameModeSelectionWidgetInstance = GameModeSelectionWidget.Get();
	RoomSettings.GameMode = GameModeSelectionWidgetInstance->GetCurrentOption<EGameModeType>();

	UPlayerCountSelectionWidget* PlayerCountSelectionWidgetInstance = PlayerCountSelectionWidget.Get();
	RoomSettings.PlayerCount = PlayerCountSelectionWidgetInstance->GetCurrentOption<int32>();

	UItemActivationSelectionWidget* ItemActivationSelectionWidgetInstance = ItemActivationSelectionWidget.Get();
	RoomSettings.bItemEnabled = ItemActivationSelectionWidgetInstance->GetCurrentOption<bool>();

	return RoomSettings;
}

void UCreateRoomWidget::OnGameModeChanged(int32 SelectedIndex)
{
	if (!GameModeSelectionWidget.IsValid() || !PlayerCountSelectionWidget.IsValid() || !ItemActivationSelectionWidget.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("OnGameModeChanged: Some widgets are not valid"));
		return;
	}

	FRoomSettings RoomSettings;
	RoomSettings.RoomName = RoomNameText ? RoomNameText->GetText().ToString() : TEXT("Default Room");
	RoomSettings.GameMode = GameModeSelectionWidget->GetCurrentOption<EGameModeType>();
	RoomSettings.PlayerCount = PlayerCountSelectionWidget->GetCurrentOption<int32>();
	RoomSettings.bItemEnabled = ItemActivationSelectionWidget->GetCurrentOption<bool>();

	if (RoomSettings.GameMode == EGameModeType::Single)
	{
		PlayerCountSelectionWidget->SetCurrentOption(TEXT("1"));
		PlayerCountSelectionWidget->SetIsEnabled(false);
	}
	else
	{
		PlayerCountSelectionWidget->SetIsEnabled(true);
		CreateRoomButton->SetIsEnabled(true);
	}

	const UEnum* EnumPtr = StaticEnum<EGameModeType>();
	FString GameModeName = EnumPtr ? EnumPtr->GetDisplayNameTextByValue(static_cast<int64>(RoomSettings.GameMode)).ToString() : TEXT("Unknown");
	UE_LOG(LogTemp, Warning, TEXT("Create Room: %s, %s, %d, %d"),
		*RoomSettings.RoomName,
		*GameModeName,
		RoomSettings.PlayerCount,
		RoomSettings.bItemEnabled);
}

void UCreateRoomWidget::ResetRoomNameText()
{
	if (RoomNameText)
	{
		RoomNameText->SetText(FText::FromString(GenerateRandomRoomName()));
	}
	UE_LOG(LogTemp, Warning, TEXT("ResetCreateRoomWidget Called"));
}

