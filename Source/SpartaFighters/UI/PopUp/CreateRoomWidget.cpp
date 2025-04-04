#include "UI/PopUp/CreateRoomWidget.h"
#include "UI/RoomWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "UI/UIObject/GameModeSelectionWidget.h"
#include "UI/UIObject/PlayerCountSelectionWidget.h"
#include "UI/UIObject/ItemActivationSelectionWidget.h"
#include "Engine/AssetManager.h"
#include "Engine/StreamableManager.h"
#include "GameFramework/HUD.h"

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
	if (!RoomWidgetClass.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("RoomWidgetClass is not valid"));
		return;
	}

	FRoomSettings NewRoomSettings = GetRoomSettings();

	UClass* WidgetClass = RoomWidgetClass.LoadSynchronous();
	if (!WidgetClass)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to load RoomWidgetClass"));
		return;
	}

	URoomWidget* RoomWidgetInstance = CreateWidget<URoomWidget>(GetWorld(), WidgetClass);
	if (RoomWidgetInstance)
	{
		FRoomInfo RoomInfo;
		RoomInfo.RoomName = NewRoomSettings.RoomName;
		RoomWidgetInstance->SetupRoom(RoomInfo);

		RoomWidgetInstance->AddToViewport();

		UE_LOG(LogTemp, Log, TEXT("Room Widget Created and Opened: %s"), *RoomInfo.RoomName);
	}

	// TODO : Hide Lobby and this Widget(self)
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
	RoomSettings.GameMode = GameModeSelectionWidgetInstance->GetCurrentOption<EGameMode>();

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
	RoomSettings.GameMode = GameModeSelectionWidget->GetCurrentOption<EGameMode>();
	RoomSettings.PlayerCount = PlayerCountSelectionWidget->GetCurrentOption<int32>();
	RoomSettings.bItemEnabled = ItemActivationSelectionWidget->GetCurrentOption<bool>();

	if (RoomSettings.GameMode == EGameMode::Single)
	{
		PlayerCountSelectionWidget->SetCurrentOption(TEXT("1"));
		PlayerCountSelectionWidget->SetIsEnabled(false);
	}
	else
	{
		PlayerCountSelectionWidget->SetIsEnabled(true);
		CreateRoomButton->SetIsEnabled(true);
	}

	const UEnum* EnumPtr = StaticEnum<EGameMode>();
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