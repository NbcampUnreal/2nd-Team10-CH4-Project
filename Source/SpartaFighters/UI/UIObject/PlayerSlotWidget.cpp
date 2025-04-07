#include "UI/UIObject/PlayerSlotWidget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"

void UPlayerSlotWidget::SetupPlayerSlot(const FString& PlayerName, UTexture2D* CharacterPreview, bool bIsReady)
{
	if (PlayerNameText)
	{
		PlayerNameText->SetText(FText::FromString(PlayerName));
	}

	if (CharacterPreviewImage && CharacterPreview)
	{
		CharacterPreviewImage->SetBrushFromTexture(CharacterPreview);
	}

	if (ReadyStatusText)
	{
		ReadyStatusText->SetText(bIsReady ? FText::FromString(TEXT("Ready")) : FText::FromString(TEXT("Not Ready")));
	}

	SetVisibility(ESlateVisibility::Visible);

}

void UPlayerSlotWidget::ClearSlot()
{
	if (PlayerNameText)
	{
		PlayerNameText->SetText(FText::FromString(TEXT("비어있음")));
	}

	if (ReadyStatusText)
	{
		ReadyStatusText->SetText(FText::FromString(TEXT("-")));
	}

	if (CharacterPreviewImage)
	{
		CharacterPreviewImage->SetBrushFromTexture(nullptr);  // 캐릭터 프리뷰 없애기
	}

	SetVisibility(ESlateVisibility::Visible);
}