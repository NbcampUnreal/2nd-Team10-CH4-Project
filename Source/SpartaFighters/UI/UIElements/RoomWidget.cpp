#include "RoomWidget.h"
#include "LobbyMenu.h"
#include "Components/TextBlock.h"
#include "Components/ScrollBox.h"
#include "Components/EditableTextBox.h"
#include "Components/Button.h"
#include "UI/UIObject/RoomChatWidget.h"
#include "UI/UIObject/MapSelectionWidget.h"
#include "Blueprint/WidgetBlueprintLibrary.h"

void URoomWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (LobbyButton)
	{
		LobbyButton->OnClicked.AddDynamic(this, &URoomWidget::OnLobbyButtonClicked);
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
	// TODO: Update the player list UI with the current room information
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
