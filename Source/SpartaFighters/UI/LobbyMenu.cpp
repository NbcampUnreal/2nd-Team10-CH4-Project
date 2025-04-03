#include "UI/LobbyMenu.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "Components/VerticalBox.h"
#include "Components/EditableTextBox.h"
#include "UI/QuitGameWidget.h"
#include "UI/PopUp/CreateRoomWidget.h"

void ULobbyMenu::NativeConstruct()
{
    Super::NativeConstruct();

    if (PlayerInfoButton)
        PlayerInfoButton->OnClicked.AddDynamic(this, &ULobbyMenu::OnPlayerInfoClicked);

    if (ShopButton)
        ShopButton->OnClicked.AddDynamic(this, &ULobbyMenu::OnShopClicked);

    if (CreateRoomButton)
        CreateRoomButton->OnClicked.AddDynamic(this, &ULobbyMenu::OnCreateRoomClicked);

    if (OptionButton)
        OptionButton->OnClicked.AddDynamic(this, &ULobbyMenu::OnOptionClicked);

    if (QuitGameButton)
        QuitGameButton->OnClicked.AddDynamic(this, &ULobbyMenu::OnQuitGameClicked);

    if (CooperativeFilterButton)
        CooperativeFilterButton->OnClicked.AddDynamic(this, &ULobbyMenu::OnCooperativeFilterClicked);

    if (BattleFilterButton)
        BattleFilterButton->OnClicked.AddDynamic(this, &ULobbyMenu::OnBattleFilterClicked);

    if (ChatTextBox)
        ChatTextBox->OnTextCommitted.AddDynamic(this, &ULobbyMenu::OnChatTextCommitted);

    if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
    {
        FInputModeUIOnly InputModeData;
        PlayerController->SetInputMode(InputModeData);
        PlayerController->bShowMouseCursor = true;
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
	// Open the Shop menu UI
}

void ULobbyMenu::OnCreateRoomClicked()
{
    UE_LOG(LogTemp, Warning, TEXT("OnCreateRoomClicked!"));
    if (!CreateRoomWidget && CreateRoomWidgetClass)
    {
        UWorld* World = GetWorld();
        if (!World) return;

        CreateRoomWidget = CreateWidget<UCreateRoomWidget>(World, CreateRoomWidgetClass);
        if (CreateRoomWidget)
        {
            CreateRoomWidget->AddToViewport();
        }
    }

    if (CreateRoomWidget)
    {
        CreateRoomWidget->ResetRoomNameText();
        CreateRoomWidget->SetVisibility(ESlateVisibility::Visible);

        if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
        {
            FInputModeUIOnly InputModeData;
            PlayerController->SetInputMode(InputModeData);
            PlayerController->bShowMouseCursor = true;
        }
    }
}

void ULobbyMenu::OnOptionClicked()
{
    UE_LOG(LogTemp, Warning, TEXT("OnOptionClicked!"));
	// Open the Option menu UI
}

void ULobbyMenu::OnQuitGameClicked()
{
    if (!QuitGameWidget && QuitGameWidgetClass)
    {
        UWorld* World = GetWorld();
        if (!World) return;

        QuitGameWidget = CreateWidget<UQuitGameWidget>(World, QuitGameWidgetClass);
        if (QuitGameWidget)
        {
            QuitGameWidget->AddToViewport();
        }
    }

    if (QuitGameWidget)
    {
        QuitGameWidget->SetVisibility(ESlateVisibility::Visible);

        if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
        {
            FInputModeUIOnly InputModeData;
            PlayerController->SetInputMode(InputModeData);
            PlayerController->bShowMouseCursor = true;
        }
    }
}

void ULobbyMenu::OnCooperativeFilterClicked()
{
    UE_LOG(LogTemp, Warning, TEXT("OnCooperativeFilterClicked!"));
    // Filter the list of Cooperative rooms
}

void ULobbyMenu::OnBattleFilterClicked()
{
    UE_LOG(LogTemp, Warning, TEXT("OnBattleFilterClicked!"));
    // Filter the list of Battle rooms
}

void ULobbyMenu::OnChatTextCommitted(const FText& Text, ETextCommit::Type CommitType)
{
    if (CommitType == ETextCommit::OnEnter)
    {
        FString ChatMessage = Text.ToString();
        UE_LOG(LogTemp, Warning, TEXT("OnChatTextCommitted ют╥б: %s"), *ChatMessage);

        // Need to add messages to a chat list

        ChatTextBox->SetText(FText::GetEmpty()); 
    }
}
