#include "UI/PopUp/CreateRoomWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "UI/UIObject/GameModeSelectionWidget.h"
#include "UI/UIObject/PlayerCountSelectionWidget.h"
#include "UI/UIObject/ItemActivationSelectionWidget.h"
// #include "SFGameInstanceSubsystem.h"

void UCreateRoomWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (CreateRoomButton)
	{
		CreateRoomButton->OnClicked.AddDynamic(this, &UCreateRoomWidget::OnCreateRoomButtonClicked);
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

	if (RoomNameText)
	{
		RoomNameText->SetText(FText::FromString(GenerateRandomRoomName()));
	}
}

void UCreateRoomWidget::OnCreateRoomButtonClicked()
{
	GetRoomSettings();

	// Request to create a room (requires integration with server)

	//USFGameInstance* GameInstance = Cast<USFGameInstance>(GetGameInstance());
	//if (GameInstance)
	//{
	//    GameInstance->CreateGameRoom(RoomName, SelectedMode, PlayerCount, bItemsEnabled);
	//}
	FRoomSettings RoomSettings = GetRoomSettings();

	UE_LOG(LogTemp, Warning, TEXT("Create Room: %s, %s, %d, %d"),
		*RoomSettings.RoomName,
		*RoomSettings.GameMode,
		RoomSettings.PlayerCount,
		RoomSettings.bItemEnabled);

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
	RoomSettings.GameMode = GameModeSelectionWidgetInstance->GetCurrentOption();
	
	UPlayerCountSelectionWidget* PlayerCountSelectionWidgetInstance = PlayerCountSelectionWidget.Get();
	RoomSettings.PlayerCount = FCString::Atoi(*PlayerCountSelectionWidgetInstance->GetCurrentOption());
	
	UItemActivationSelectionWidget* ItemActivationSelectionWidgetInstance = ItemActivationSelectionWidget.Get();
	RoomSettings.bItemEnabled = ItemActivationSelectionWidgetInstance->GetCurrentOption() == TEXT("ON");

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
	RoomSettings.GameMode = GameModeSelectionWidget->GetCurrentOption();
	RoomSettings.PlayerCount = FCString::Atoi(*PlayerCountSelectionWidget->GetCurrentOption());
	RoomSettings.bItemEnabled = ItemActivationSelectionWidget->GetCurrentOption() == TEXT("ON");

	if (RoomSettings.GameMode == TEXT("Single"))
	{
		PlayerCountSelectionWidget->SetCurrentOption(TEXT("1"));
		PlayerCountSelectionWidget->SetIsEnabled(false);
	}
	else
	{
		PlayerCountSelectionWidget->SetIsEnabled(true);
		CreateRoomButton->SetIsEnabled(true);
	}

	UE_LOG(LogTemp, Warning, TEXT("Create Room: %s, %s, %d, %d"),
		*RoomSettings.RoomName,
		*RoomSettings.GameMode,
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