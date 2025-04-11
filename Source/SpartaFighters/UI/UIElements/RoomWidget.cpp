#include "RoomWidget.h"
#include "LobbyMenu.h"
#include "Framework/SFGameInstance.h"
#include "Framework/SFGameInstanceSubsystem.h"
#include "Framework/SFLobbyPlayerController.h"
#include "Framework/SFGameStateBase.h"
#include "Framework/SFPlayerState.h"

#include "Components/Button.h"
#include "UI/UIObject/PlayerSlotWidget.h"
#include "UI/UIManager/UIManager.h"
#include "Blueprint/WidgetBlueprintLibrary.h"

void URoomWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (ShopButton)
	{
		ShopButton->OnClicked.AddDynamic(this, &URoomWidget::OnShopButtonClicked);
	}
	if (PlayerInfoButton)
	{
		PlayerInfoButton->OnClicked.AddDynamic(this, &URoomWidget::OnPlayerInfoButtonClicked);
	}
	if (OptionButton)
	{
		OptionButton->OnClicked.AddDynamic(this, &URoomWidget::OnOptionButtonClicked);
	}
	if (LobbyButton)
	{
		LobbyButton->OnClicked.AddDynamic(this, &URoomWidget::OnLobbyButtonClicked);
	}
	if (SelectMapButton)
	{
		SelectMapButton->OnClicked.AddDynamic(this, &URoomWidget::OnSelectMapButtonClicked);
	}
	
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(
		TimerHandle, 
		this,
		&URoomWidget::UpdatePlayerSlots, 
		0.2f, 
		false);
}

void URoomWidget::NativeDestruct()
{
	Super::NativeDestruct();

	if (ShopButton)
	{
		ShopButton->OnClicked.RemoveDynamic(this, &URoomWidget::OnShopButtonClicked);
	}
	if (PlayerInfoButton)
	{
		PlayerInfoButton->OnClicked.RemoveDynamic(this, &URoomWidget::OnPlayerInfoButtonClicked);
	}
	if (OptionButton)
	{
		OptionButton->OnClicked.RemoveDynamic(this, &URoomWidget::OnOptionButtonClicked);
	}
	if (LobbyButton)
	{
		LobbyButton->OnClicked.RemoveDynamic(this, &URoomWidget::OnLobbyButtonClicked);
	}
	if (SelectMapButton)
	{
		SelectMapButton->OnClicked.RemoveDynamic(this, &URoomWidget::OnSelectMapButtonClicked);
	}
}

void URoomWidget::OnShopButtonClicked()
{
	UE_LOG(LogTemp, Warning, TEXT("OnShopClicked!"));
	if (UUIManager* UIManager = ResolveUIManager())
	{
		UIManager->ShowShopMenu();
	}
}

void URoomWidget::OnPlayerInfoButtonClicked()
{
	UE_LOG(LogTemp, Log, TEXT("OnPlayerInfoButtonClicked"));
}

void URoomWidget::OnOptionButtonClicked()
{
	UE_LOG(LogTemp, Log, TEXT("OnOptionButtonClicked"));
}

void URoomWidget::OnLobbyButtonClicked()
{
	UE_LOG(LogTemp, Log, TEXT("OnLobbyButtonClicked"));
	if (USFGameInstance* GameInstance = Cast<USFGameInstance>(GetGameInstance()))
	{
		if (USFGameInstanceSubsystem* Subsystem = GameInstance->GetSubsystem<USFGameInstanceSubsystem>())
		{
			const FString LobbyMapName = TEXT("LobbyMenu");
			Subsystem->ChangeLevelByMapName(LobbyMapName);
		}
	}
}

void URoomWidget::OnSelectMapButtonClicked()
{
	UE_LOG(LogTemp, Log, TEXT("OnSelectMapButtonClicked"));

	if (USFGameInstance* GameInstance = Cast<USFGameInstance>(GetGameInstance()))
	{
		if (USFGameInstanceSubsystem* Subsystem = GameInstance->GetSubsystem<USFGameInstanceSubsystem>())
		{
			if (UUIManager* UIManager = Subsystem->GetUIManager())
			{
				UIManager->ShowMapSelectionWidget(Subsystem->GetCurrentGameMode());
			}
		}
	}
}

void URoomWidget::UpdatePlayerSlots()
{
	UE_LOG(LogTemp, Warning, TEXT("UpdatePlayerSlots Called"));

	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}
	AGameStateBase* GameState = World->GetGameState();
	if (!GameState)
	{
		return;
	}

	const TArray<APlayerState*>& PlayerStates = GameState->PlayerArray;
	TArray<UPlayerSlotWidget*> Slots = { ClientSlot1, ClientSlot2, ClientSlot3, ClientSlot4 };

	for (int32 i = 0; i < PlayerStates.Num(); ++i)
	{
		if (!Slots.IsValidIndex(i)) continue;

		if (PlayerStates.IsValidIndex(i))
		{
			ASFPlayerState* SFPlayerState = Cast<ASFPlayerState>(PlayerStates[i]);
			if (SFPlayerState)
			{
				const FString& PlayerID = SFPlayerState->PlayerUniqueID;
				const FString& CharacterTexturePath = SFPlayerState->CharacterTexturePath;

				Slots[i]->SetupPlayerSlot(PlayerID, CharacterTexturePath, /* bIsReady */ false);
			}
		}
		else
		{
			Slots[i]->SetEmpty();
		}
	}
}
