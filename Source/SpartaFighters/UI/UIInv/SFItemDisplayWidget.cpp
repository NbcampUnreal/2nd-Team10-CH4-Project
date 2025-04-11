#include "UI/UIInv/SFItemDisplayWidget.h"
#include "Items/SFItemBase.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "UObject/SoftObjectPtr.h"
#include "Styling/SlateBrush.h"

void USFItemDisplayWidget::NativeConstruct()
{
	Super::NativeConstruct();

	//Check item image and item text
	if (!ItemIconImage || !ItemNameText)
	{
		UE_LOG(LogTemp, Warning, TEXT("Image or Textblock is not binded."));
	}
}

void USFItemDisplayWidget::SetItemInfo(USFItemBase* Item)
{
	CurrentItem = Item;

	if (ItemIconImage && Item)
	{
		ItemIconImage->SetBrushFromSoftTexture(Item->ItemIcon);
	}

	if (ItemNameText && Item)
	{
		ItemNameText->SetText(FText::FromName(Item->ItemName));
	}
}