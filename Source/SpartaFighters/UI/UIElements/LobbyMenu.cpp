#include "LobbyMenu.h"
#include "Framework/SFGameInstance.h"
#include "Framework/SFGameInstanceSubsystem.h"
#include "ShopMenu.h"
#include "Components/Button.h"

#include "UI/UIObject/PlayerSimpleInfoWidget.h"
//#include "UI/UIObject/GlobalChatWidget.h"
#include "UI/PopUp/QuitGameWidget.h"
#include "UI/UIManager/UIManager.h"

#include "Engine/AssetManager.h"
#include "Engine/StreamableManager.h"

void ULobbyMenu::NativeConstruct()
{
	Super::NativeConstruct();

	if (PlayButton)
	{
		PlayButton->OnClicked.AddDynamic(this, &ULobbyMenu::OnPlayButtonClicked);
	}
	if (PlayerInfoButton)
	{
		PlayerInfoButton->OnClicked.AddDynamic(this, &ULobbyMenu::OnPlayerInfoClicked);
	}
	if (ShopButton)
	{
		ShopButton->OnClicked.AddDynamic(this, &ULobbyMenu::OnShopClicked);
	}
	if (OptionButton)
	{
		OptionButton->OnClicked.AddDynamic(this, &ULobbyMenu::OnOptionClicked);
	}
	if (QuitGameButton)
	{
		QuitGameButton->OnClicked.AddDynamic(this, &ULobbyMenu::OnQuitGameClicked);
	}

	if (QuitGameWidgetClass.IsNull())
	{
		UE_LOG(LogTemp, Error, TEXT("QuitGameWidgetClass is NULL in NativeConstruct"));
	}
}

void ULobbyMenu::OnPlayButtonClicked()
{
	UE_LOG(LogTemp, Warning, TEXT("OnPlayButtonClicked!"));
	if (USFGameInstance* GameInstance = Cast<USFGameInstance>(GetGameInstance()))
	{
		if (USFGameInstanceSubsystem* Subsystem = GameInstance->GetSubsystem<USFGameInstanceSubsystem>())
		{
			const FString RoomMapName = TEXT("RoomMenu");

			Subsystem->ChangeLevelByMapName(RoomMapName);
		}
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
	if (UUIManager* UIManager = ResolveUIManager())
	{
		UIManager->ShowShopMenu(); 
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
