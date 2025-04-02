#include "UI/PopUp/CreateRoomWidget.h"
#include "Components/ComboBoxString.h"
#include "Components/CheckBox.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
// #include "SFGameInstanceSubsystem.h"

void UCreateRoomWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (CreateRoomButton)
    {
        CreateRoomButton->OnClicked.AddDynamic(this, &UCreateRoomWidget::OnCreateRoomButtonClicked);
    }
    if (ModeComboBox)
    {
        ModeComboBox->AddOption(TEXT("Single"));
        ModeComboBox->AddOption(TEXT("Cooperative"));
        ModeComboBox->AddOption(TEXT("Battle"));
        ModeComboBox->SetSelectedIndex(0);
    }
    if (PlayerCountComboBox)
    {
        PlayerCountComboBox->AddOption(TEXT("1"));
        PlayerCountComboBox->AddOption(TEXT("2"));
        PlayerCountComboBox->AddOption(TEXT("3"));
        PlayerCountComboBox->AddOption(TEXT("4"));
        PlayerCountComboBox->SetSelectedIndex(0);
    }
    if (RoomNameText)
    {
        RoomNameText->SetText(FText::FromString(GenerateRandomRoomName()));
    }
}

FString UCreateRoomWidget::GenerateRandomRoomName()
{
    static TArray<FString> Adjectives = { TEXT("Quick"), TEXT("Strong"), TEXT("Strategic"), TEXT("Mysterious") };
    static TArray<FString> Nouns = { TEXT("Combat"), TEXT("Challenges"), TEXT("Showdown"), TEXT("Arena") };

    int32 RandomAdjIndex = FMath::RandRange(0, Adjectives.Num() - 1);
    int32 RandomNounIndex = FMath::RandRange(0, Nouns.Num() - 1);

    return Adjectives[RandomAdjIndex] + TEXT(" ") + Nouns[RandomNounIndex];
}

void UCreateRoomWidget::OnCreateRoomButtonClicked()
{
    FString SelectedMode = ModeComboBox->GetSelectedOption();
    int32 PlayerCount = FCString::Atoi(*PlayerCountComboBox->GetSelectedOption());
    bool bItemsEnabled = ItemCheckBox->IsChecked();
    FString RoomName = RoomNameText->GetText().ToString();
    // Request to create a room (requires integration with server)

    //USFGameInstance* GameInstance = Cast<USFGameInstance>(GetGameInstance());
    //if (GameInstance)
    //{
    //    GameInstance->CreateGameRoom(RoomName, SelectedMode, PlayerCount, bItemsEnabled);
    //}
	UE_LOG(LogTemp, Warning, TEXT("Create Room: %s, %s, %d, %d"), *RoomName, *SelectedMode, PlayerCount, bItemsEnabled);
}

void UCreateRoomWidget::ResetCreateRoomWidget()
{
    UE_LOG(LogTemp, Warning, TEXT("ResetCreateRoomWidget Called"));
    if (CreateRoomButton)
    {
        CreateRoomButton->OnClicked.AddDynamic(this, &UCreateRoomWidget::OnCreateRoomButtonClicked);
    }
    if (ModeComboBox)
    {
        ModeComboBox->SetSelectedIndex(0);
    }
    if (PlayerCountComboBox)
    {
        PlayerCountComboBox->SetSelectedIndex(0);
    }
    if (RoomNameText)
    {
        RoomNameText->SetText(FText::FromString(GenerateRandomRoomName()));
    }
}