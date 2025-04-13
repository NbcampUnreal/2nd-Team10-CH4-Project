#include "RoomWidget.h"
#include "LobbyMenu.h"

#include "Framework/SFGameInstance.h"
#include "Framework/SFGameInstanceSubsystem.h"
#include "Framework/SFRoomPlayercontroller.h"
#include "Framework/SFGameStateBase.h"
#include "Framework/SFPlayerState.h"

#include "Components/Button.h"
#include "Components/UniformGridPanel.h"

#include "UI/UIObject/PlayerSlotWidget.h"
#include "UI/UIManager/UIManager.h"

#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Kismet/GameplayStatics.h"

void URoomWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (ShopButton)
	{
		ShopButton->OnClicked.AddUniqueDynamic(this, &URoomWidget::OnShopButtonClicked);
	}
	if (PlayerInfoButton)
	{
		PlayerInfoButton->OnClicked.AddUniqueDynamic(this, &URoomWidget::OnPlayerInfoButtonClicked);
	}
	if (OptionButton)
	{
		OptionButton->OnClicked.AddUniqueDynamic(this, &URoomWidget::OnOptionButtonClicked);
	}
	if (LobbyButton)
	{
		LobbyButton->OnClicked.AddUniqueDynamic(this, &URoomWidget::OnLobbyButtonClicked);
	}
	if (SelectMapButton)
	{
		SelectMapButton->OnClicked.AddUniqueDynamic(this, &URoomWidget::OnSelectMapButtonClicked);
	}
	if (ReadyButton)
	{
		ReadyButton->OnClicked.AddUniqueDynamic(this, &URoomWidget::OnReadyButtonClicked);
	}

	GetWorld()->GetTimerManager().SetTimer(
		UpdatePlayerSlotTimerHandle,
		this,
		&URoomWidget::UpdatePlayerSlots,
		1.0f,
		true
	);
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

	if (GetWorld())
	{
		GetWorld()->GetTimerManager().ClearTimer(UpdatePlayerSlotTimerHandle);
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

	if (USFGameInstance* GameInstance = Cast<USFGameInstance>(GetGameInstance()))
	{
		if (USFGameInstanceSubsystem* Subsystem = GameInstance->GetSubsystem<USFGameInstanceSubsystem>())
		{
			if (UUIManager* UIManager = Subsystem->GetUIManager())
			{
				UIManager->ShowSelectCharacterWidget();
			}
		}
	}
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

void URoomWidget::OnReadyButtonClicked()
{
	APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
	if (ASFRoomPlayerController* SFPC = Cast<ASFRoomPlayerController>(PC))
	{
		SFPC->Server_SetReady(true);
	}
}


void URoomWidget::UpdatePlayerSlots()
{
	if (!PlayerGridPanel)
	{
		return;
	}

	PlayerGridPanel->ClearChildren();

	const UWorld* World = GetWorld();
	if (World == nullptr)
	{
		ensure(false);
		return;
	}
	const AGameStateBase* GS = World->GetGameState();
	if (GS == nullptr)
	{
		ensure(false);
		return;
	}
	const APlayerController* PC = UGameplayStatics::GetPlayerController(World, 0);
	if (PC == nullptr)
	{
		ensure(false);
		return;
	}
	const APlayerState* PS = PC->PlayerState;
	if (PS == nullptr)
	{
		ensure(false);
		return;
	}
	const TArray<APlayerState*>& Players = GS->PlayerArray;
	if (Players.Num() == 0)
	{
		return;
	}
	const FString LocalID = PS->GetPlayerName();

	int32 Index = 0;
	bool bAllReady = true;
	bool bLocalIsReady = false;
	bool bIsLocalOwner = false;

	for (APlayerState* CurrentPlayerState : Players)
	{
		ASFPlayerState* SFPS = Cast<ASFPlayerState>(CurrentPlayerState);
		if (!SFPS) continue;

		UPlayerSlotWidget* PlayerSlotWidgetInstance = CreateWidget<UPlayerSlotWidget>(this, PlayerSlotWidgetClass);
		if (PlayerSlotWidgetInstance)
		{
			PlayerSlotWidgetInstance->SetupPlayerSlot(SFPS->GetPlayerName(), "nullptr", SFPS->bIsReady);
			PlayerSlotWidgetInstance->UpdateRoomOwner(SFPS->bIsRoomOwner ? SFPS->GetPlayerName() : TEXT(""));
			PlayerGridPanel->AddChildToUniformGrid(PlayerSlotWidgetInstance, 0, Index);
			Index++;
		}

		if (!SFPS->bIsAI && !SFPS->bIsReady)
		{
			bAllReady = false;
		}

		if (SFPS->GetPlayerName() == LocalID)
		{
			bLocalIsReady = SFPS->bIsReady;
			bIsLocalOwner = SFPS->bIsRoomOwner;
		}
	}

	UpdateUIState();

	if (SelectMapButton)
	{
		SelectMapButton->SetIsEnabled(bAllReady && bIsLocalOwner);
	}
}

void URoomWidget::UpdateUIState()
{
	APlayerController* PC = GetOwningPlayer();
	if (!PC)
	{
		return;
	}

	ASFPlayerState* SFPS = PC->GetPlayerState<ASFPlayerState>();
	if (!SFPS)
	{
		return;
	}

	const bool bLock = SFPS->bIsReady;

	if (ShopButton) ShopButton->SetIsEnabled(!bLock);
	if (PlayerInfoButton) PlayerInfoButton->SetIsEnabled(!bLock);
	if (OptionButton) OptionButton->SetIsEnabled(!bLock);
	if (LobbyButton) LobbyButton->SetIsEnabled(!bLock);
}
