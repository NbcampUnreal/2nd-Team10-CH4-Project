#include "RoomWidget.h"
#include "LobbyMenu.h"
#include "Framework/SFGameInstance.h"
#include "Framework/SFGameInstanceSubsystem.h"
#include "Framework/SFLobbyPlayerController.h"
#include "Framework/SFGameStateBase.h"

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
	if (StartButton)
	{
		StartButton->OnClicked.AddDynamic(this, &URoomWidget::OnStartButtonClicked);
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

void URoomWidget::OnStartButtonClicked()
{
	UE_LOG(LogTemp, Log, TEXT("OnStartButtonClicked"));
}
