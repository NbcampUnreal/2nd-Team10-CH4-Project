#include "UI/UIObject/SelectCharacterWidget.h"
#include "UI/UIObject/CharacterSlotWidget.h"
#include "Components/UniformGridPanel.h"
#include "Components/ComboBoxString.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "Framework/SFRoomPlayerController.h"
#include "Framework/SFLobbyPlayerController.h"

void USelectCharacterWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (ConfirmButton)
	{
		//ConfirmButton->OnClicked.AddUniqueDynamic(this, &USelectCharacterWidget::OnConfirmClicked);
	}

	PopulateCharacterGrid();
}

void USelectCharacterWidget::NativeDestruct()
{
	Super::NativeDestruct();

	if (ConfirmButton)
	{
		//ConfirmButton->OnClicked.RemoveDynamic(this, &USelectCharacterWidget::OnConfirmClicked);
	}
}

void USelectCharacterWidget::PopulateCharacterGrid()
{
	if (!CharacterDataTable || !CharacterSlotWidgetClass)
	{
		return;
	}

	TArray<FCharacterDataRow*> AllCharacters;
	CharacterDataTable->GetAllRows<FCharacterDataRow>(TEXT("Context"), AllCharacters);

	const int32 Columns = 4;
	for (int32 i = 0; i < AllCharacters.Num(); ++i)
	{
		UCharacterSlotWidget* CharacterSlotWidget = CreateWidget<UCharacterSlotWidget>(this, CharacterSlotWidgetClass);
		if (CharacterSlotWidget)
		{
			CharacterSlotWidget->Setup(*AllCharacters[i]);
			CharacterSlotWidget->OnClicked.AddUniqueDynamic(this, &USelectCharacterWidget::OnCharacterSlotClicked);

			CharacterGrid->AddChildToUniformGrid(CharacterSlotWidget, i / Columns, i % Columns);
		}
	}
}

void USelectCharacterWidget::OnCharacterSlotClicked(FName RowName)
{
	SelectedCharacterRow = RowName;
	// TODO: UI 하이라이트 갱신 or 설명 표시
}

//void USelectCharacterWidget::OnConfirmClicked()
//{
//	APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
//	if (ASFRoomPlayerController* SFRoomPC = Cast<ASFRoomPlayerController>(PC))
//	{
//		FString CommonItem = CommonItemComboBox->GetSelectedOption();
//		FString ExclusiveItem = ExclusiveItemComboBox->GetSelectedOption();
//		FString CosmeticItem = CosmeticItemComboBox->GetSelectedOption();
//
//		SFRoomPC->Server_SelectCharacter(
//			SelectedCharacterRow,
//			*CommonItem,
//			*ExclusiveItem,
//			*CosmeticItem
//		);
//	}
//	else if(ASFLobbyPlayerController * SFLobbyPC = Cast<ASFLobbyPlayerController>(PC))
//	{
//		FString CommonItem = CommonItemComboBox->GetSelectedOption();
//		FString ExclusiveItem = ExclusiveItemComboBox->GetSelectedOption();
//		FString CosmeticItem = CosmeticItemComboBox->GetSelectedOption();
//
//		SFLobbyPC->Server_SelectCharacter(
//			SelectedCharacterRow,
//			*CommonItem,
//			*ExclusiveItem,
//			*CosmeticItem
//		);
//	}
//}

void USelectCharacterWidget::UpdateAvailableCharacters()
{
	if (!CharacterDataTable)
	{
		return;
	}

	TArray<FCharacterDataRow*> AllCharacters;
	static const FString Context = TEXT("CharacterContext");
	CharacterDataTable->GetAllRows(Context, AllCharacters);

	AvailableCharacters.Empty();

	for (FCharacterDataRow* Row : AllCharacters)
	{
		if (!Row)
		{
			continue;
		}

		if (IsCharacterUnlocked(Row->CharacterName)) 
		{
			AvailableCharacters.Add(Row);
		}
	}

	PopulateCharacterGrid(); 
}

bool USelectCharacterWidget::IsCharacterUnlocked(FName CharacterName) const
{
	/*if (USFSaveGame* SaveGame = GetPlayerSaveData())
	{
		return SaveGame->UnlockedCharacters.Contains(CharacterName);
	}*/

	return CharacterName == "DefaultFighter";
}
