#include "UI/UIObject/CharacterSlotWidget.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Framework/SFRoomPlayerController.h"
#include "Kismet/GameplayStatics.h"

void UCharacterSlotWidget::Setup(const FCharacterDataRow& InData)
{
	RowName = InData.CharacterName;

	if (CharacterThumbnail && InData.Thumbnail)
	{
		CharacterThumbnail->SetBrushFromTexture(InData.Thumbnail);
	}

	if (SelectButton)
	{
		SelectButton->OnClicked.AddUniqueDynamic(this, &UCharacterSlotWidget::HandleClick);
	}
}

void UCharacterSlotWidget::HandleClick()
{
	APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
	if (ASFRoomPlayerController* SFRoomPC = Cast<ASFRoomPlayerController>(PC))
	{
		/*FString CommonItem = CommonItemComboBox->GetSelectedOption();
		FString ExclusiveItem = ExclusiveItemComboBox->GetSelectedOption();
		FString CosmeticItem = CosmeticItemComboBox->GetSelectedOption();*/

		SFRoomPC->Server_SelectCharacter(
			RowName/**,
			CommonItem,
			*ExclusiveItem,
			*CosmeticItem*/
		);
	}
}

