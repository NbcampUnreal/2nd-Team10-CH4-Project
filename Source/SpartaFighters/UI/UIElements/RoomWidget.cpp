#include "RoomWidget.h"
#include "LobbyMenu.h"

#include "Components/TextBlock.h"
#include "Components/ScrollBox.h"
#include "Components/EditableTextBox.h"
#include "Components/Button.h"
#include "Components/UniformGridPanel.h"
#include "Components/UniformGridSlot.h"

#include "UI/UIObject/RoomChatWidget.h"
#include "UI/UIObject/MapSelectionWidget.h"
#include "UI/UIObject/PlayerSlotWidget.h"
#include "Blueprint/WidgetBlueprintLibrary.h"

#include "DataTypes/PlayerInfo.h"
#include "DataTypes/GameModeType.h"

void URoomWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (LobbyButton)
	{
		LobbyButton->OnClicked.AddDynamic(this, &URoomWidget::OnLobbyButtonClicked);
	}
	if (ReadyOrStartButton)
	{
		ReadyOrStartButton->OnClicked.AddDynamic(this, &URoomWidget::OnReadyOrStartButtonClicked);
	}
}

void URoomWidget::SetupRoom(const FRoomInfo& RoomInfo)
{
	CurrentRoomInfo = RoomInfo;

	// Display room information in formatted text
	if (RoomNameText)
	{
		FString GameModeString;
		switch (RoomInfo.GameMode)
		{
		case EGameModeType::Cooperative:
			GameModeString = TEXT("Cooperative");
			break;
		case EGameModeType::Battle:
			GameModeString = TEXT("Battle");
			break;
		case EGameModeType::Single:
			GameModeString = TEXT("Single");
			break;
		default:
			GameModeString = TEXT("Unknown");
			break;
		}

		FString DisplayText = FString::Printf(TEXT("%d   |   %s   |   %s  "),
			RoomInfo.RoomID,
			*RoomInfo.RoomName,
			*GameModeString
		);

		RoomNameText->SetText(FText::FromString(DisplayText));
	}

	if (MapSelectionWidgetClass)
	{
		MapSelectionWidgetClass->SetGameMode(RoomInfo.GameMode);
	}

	UpdatePlayerList();
}

void URoomWidget::SetPlayerList(const TArray<FPlayerInfo>& NewPlayerList)
{
	PlayerList = NewPlayerList;
	UpdatePlayerList();
}

void URoomWidget::UpdatePlayerList()
{
	UE_LOG(LogTemp, Warning, TEXT("PlayerList.Num() = %d"), PlayerList.Num());
	for (const auto& Player : PlayerList)
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerName = %s, IsReady = %d"), *Player.PlayerID, Player.bIsReady);
	}

	bool bIsSingleMode = (CurrentRoomInfo.GameMode == EGameModeType::Single);

	int32 RequiredSlots = bIsSingleMode ? 1 : MaxPlayers;

	while (PlayerSlots.Num() < RequiredSlots)
	{
		if (PlayerSlotWidgetClass)
		{
			UPlayerSlotWidget* NewSlot = CreateWidget<UPlayerSlotWidget>(GetWorld(), PlayerSlotWidgetClass);
			if (NewSlot)
			{
				PlayerSlots.Add(NewSlot);
				int32 Index = PlayerSlots.Num() - 1;
				PlayerGridPanel->AddChildToUniformGrid(NewSlot, Index / NumColumns, Index % NumColumns);
			}
		}
	}

	// Setup Slots
	for (int32 i = 0; i < RequiredSlots; ++i)
	{
		if (i < PlayerList.Num())
		{
			const FPlayerInfo& PlayerInfo = PlayerList[i];
			UPlayerSlotWidget* CurrentPlayerSlot = PlayerSlots[i];
			if (CurrentPlayerSlot)
			{
				CurrentPlayerSlot->SetupPlayerSlot(PlayerInfo.PlayerID, PlayerInfo.CharacterTexture, PlayerInfo.bIsReady);
				CurrentPlayerSlot->SetVisibility(ESlateVisibility::Visible);
			}
		}
		else
		{
			UPlayerSlotWidget* EmptyPlayerSlot = PlayerSlots[i];
			if (EmptyPlayerSlot)
			{
				EmptyPlayerSlot->SetEmpty();
				EmptyPlayerSlot->SetVisibility(ESlateVisibility::Visible);
			}
		}
	}
}

void URoomWidget::OnLobbyButtonClicked()
{
	UE_LOG(LogTemp, Log, TEXT("Leaving Room: %s"), *CurrentRoomInfo.RoomName);
	APlayerController* PlayerController = GetOwningPlayer();
	if (PlayerController)
	{
		TArray<UUserWidget*> FoundWidgets;
		UWidgetBlueprintLibrary::GetAllWidgetsOfClass(GetWorld(), FoundWidgets, ULobbyMenu::StaticClass(), false);

		for (UUserWidget* Widget : FoundWidgets)
		{
			if (ULobbyMenu* LobbyMenu = Cast<ULobbyMenu>(Widget))
			{
				LobbyMenu->SetVisibility(ESlateVisibility::Visible);
				break;
			}
		}
	}

	RemoveFromParent();
}

void URoomWidget::OnReadyOrStartButtonClicked()
{
	// Create a NewPlayerList for test
	TArray<FPlayerInfo> NewPlayerList;

	FPlayerInfo Player;
	Player.PlayerID = TEXT("TestPlayer");
	Player.bIsReady = true;

	NewPlayerList.Add(Player);
	// 
	SetPlayerList(NewPlayerList);
}
