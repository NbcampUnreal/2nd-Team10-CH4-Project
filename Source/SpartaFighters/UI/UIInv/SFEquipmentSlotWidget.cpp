
#include "UI/UIInv/SFEquipmentSlotWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Items/EquipItems/SFEquipableBase.h"
#include "Inventory/SFInventoryComponent.h"

//USFEquipmentSlotWidget::USFEquipmentSlotWidget()
//{
//	//Set default
//	CurrentEquippedItem = nullptr;
//	CurrentSlotType = SFEquipSlot::CommonSlot;
//}
//
//void USFEquipmentSlotWidget::NativeConstruct()
//{
//	Super::NativeConstruct();
//
//	//Unequip Click event
//	if (UnequipButton)
//	{
//		UnequipButton->OnClicked.AddDynamic(this, &USFEquipmentSlotWidget::NativeOnUnequipButtonClicked);
//	}
//	else
//	{
//		UE_LOG(LogTemp, Warning, TEXT("UnequipButton"));
//	}
//}
//
//void USFEquipmentSlotWidget::SetEquippedItem(const USFEquipableBase* Item)
//{
//	CurrentEquippedItem = Item;
//
//	if (EquipItemIcon)
//	{
//		EquipItemIcon->SetBrushFromTexture(Item ? Item->ItemIcon : nullptr);
//		EquipItemIcon->SetVisibility(Item ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
//	}
//
//	if (UnequipButton)
//	{
//		UnequipButton->SetVisibility(Item ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
//	}
//}
//
//void USFEquipmentSlotWidget::SetSlotType(SFEquipSlot SlotType)
//{
//	CurrentSlotType = SlotType;
//	if (EquipSlotName)
//	{
//		FString SlotNameString;
//		switch (SlotType)
//		{
//		case SFEquipSlot::CommonSlot:
//			SlotNameString = TEXT("Common");
//			break;
//		case SFEquipSlot::ExclusiveSlot:
//			SlotNameString = TEXT("Exclusive");
//			break;
//		case SFEquipSlot::CosmeticSlot:
//			SlotNameString = TEXT("Cosmetic");
//			break;
//		default:
//			SlotNameString = TEXT("Unknown");
//			break;
//		}
//		EquipSlotName->SetText(FText::FromString(SlotNameString));
//	}
//}
//
////void USFEquipmentSlotWidget::SetUnequipDelegate(FOnUnequipItemSignature Delegate)
////{
////	OnUnequipButtonClickedDelegate = Delegate;
////}
////
////void USFEquipmentSlotWidget::NativeOnUnequipButtonClicked()
////{
////	
////	if (OnUnequipButtonClickedDelegate.IsBound())
////	{
////		OnUnequipButtonClickedDelegate.Execute(CurrentSlotType);
////	}
////}