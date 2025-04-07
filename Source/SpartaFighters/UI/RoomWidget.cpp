#include "UI/RoomWidget.h"
#include "Components/TextBlock.h"
#include "Components/ScrollBox.h"
#include "Components/EditableTextBox.h"
#include "Components/UniformGridPanel.h"
#include "Components/Button.h"
#include "UI/UIObject/RoomChatWidget.h"
#include "UI/UIObject/MapSelectionWidget.h"
#include "UI/UIObject/PlayerSlotWidget.h"
#include "UI/LobbyMenu.h"
#include "Blueprint/WidgetBlueprintLibrary.h"

void URoomWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (LobbyButton)
	{
		LobbyButton->OnClicked.AddDynamic(this, &URoomWidget::OnLobbyButtonClicked);
	}

	for (int32 i = 0; i < MaxPlayers; ++i)
	{
		if (PlayerSlotWidgetClass)
		{
			UPlayerSlotWidget* NewSlot = CreateWidget<UPlayerSlotWidget>(GetWorld(), PlayerSlotWidgetClass);
			PlayerGrid->AddChildToUniformGrid(NewSlot, i / NumColumns, i % NumColumns);
			PlayerSlots.Add(NewSlot);
		}
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

void URoomWidget::UpdatePlayerList()
{
	if (!PlayerGridPanel) return;

	PlayerGridPanel->ClearChildren();
	PlayerSlots.Empty();

	const int32 MaxPlayers = 4; 
	for (int32 Index = 0; Index < MaxPlayers; ++Index)
	{
		UPlayerSlotWidget* NewSlot = CreateWidget<UPlayerSlotWidget>(GetWorld(), PlayerSlotWidgetClass);
		if (!NewSlot) continue;

		if (Index < PlayerList.Num())
		{
			NewSlot->SetupPlayerSlot(PlayerList[Index]);
		else
		{
			NewSlot->SetupEmptySlot();
		}

		int32 Row = Index / 2;
		int32 Col = Index % 2;
		PlayerGridPanel->AddChildToUniformGrid(NewSlot, Row, Col);

		PlayerSlots.Add(NewSlot);
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
