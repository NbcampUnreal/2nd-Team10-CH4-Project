#include "UI/UIObject/PlayerSlotWidget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/Border.h"

void UPlayerSlotWidget::SetupPlayerSlot(const FString& PlayerName, const FString& CharacterPreviewPath, bool bIsReady)
{
	if (PlayerNameText)
	{
		PlayerNameText->SetText(FText::FromString(PlayerName));
		SlotPlayerID = PlayerName;
	}
	/*if (CharacterPreviewImage && CharacterPreview)
	{
		CharacterPreviewImage->SetBrushFromTexture(CharacterPreview);
	}*/
	if (ReadyStateText)
	{
		ReadyStateText->SetText(bIsReady ? FText::FromString(TEXT("Ready")) : FText::FromString(TEXT("Not Ready")));
	}

	SetVisibility(ESlateVisibility::Visible);
}

void UPlayerSlotWidget::SetReadyState(bool bIsReady)
{
	if (ReadyStateText)
	{
		ReadyStateText->SetText(FText::FromString(bIsReady ? TEXT("Ready") : TEXT("Not Ready")));
	}
	if (SlotBorder)
	{
		SlotBorder->SetBrushColor(bIsReady ? FLinearColor::Green : FLinearColor::White);
	}
}

void UPlayerSlotWidget::SetEmpty()
{
	if (CharacterPreviewImage)
	{
		CharacterPreviewImage->SetBrushFromTexture(nullptr);
	}
	if (PlayerNameText)
	{
		PlayerNameText->SetText(FText::FromString(TEXT("Empty Slot")));
	}
	if (ReadyStateText)
	{
		ReadyStateText->SetText(FText::GetEmpty());
	}
	if (SlotBorder)
	{
		SlotBorder->SetBrushColor(FLinearColor::Gray);
	}
}

void UPlayerSlotWidget::UpdateRoomOwner(const FString& OwnerPlayerID)
{
	if (OwnerPlayerID.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("OwnerPlayerID.IsEmpty!"));
		return;
	}

	if (HostIcon)
	{
		HostIcon->SetVisibility(OwnerPlayerID == SlotPlayerID ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
	}
}
