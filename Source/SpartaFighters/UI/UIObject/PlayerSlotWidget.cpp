#include "UI/UIObject/PlayerSlotWidget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/Border.h"
#include "DataTable/CharacterDataRow.h"

#include "Framework/SFPlayerState.h"

void UPlayerSlotWidget::SetupPlayerSlot(ASFPlayerState* PlayerState, UDataTable* InCharacterDataTable)
{
	if (!PlayerState || !InCharacterDataTable)
	{
		return;
	}

	if (PlayerNameText)
	{
		PlayerNameText->SetText(FText::FromString(PlayerState->GetUniqueID()));
		SlotPlayerID = PlayerState->GetUniqueID();
	}

	if (ReadyStateText)
	{
		ReadyStateText->SetText(PlayerState->bIsReady ? FText::FromString(TEXT("Ready")) : FText::FromString(TEXT("Not Ready")));
	}

	const FString ContextString(TEXT("CharacterDataLookup"));
	if (const FCharacterDataRow* CharacterData = InCharacterDataTable->FindRow<FCharacterDataRow>(PlayerState->SelectedCharacterRow, ContextString))
	{
		if (CharacterPreviewImage && CharacterData->Thumbnail)
		{
			CharacterPreviewImage->SetBrushFromTexture(CharacterData->Thumbnail);
		}
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
