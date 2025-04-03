


#include "Items/SFItemBase.h"

USFItemBase::USFItemBase():
	ItemIcon(nullptr),
	ItemName(NAME_None),
	ItemDescription(FText::GetEmpty()),
	ItemType(EItemType::Common),
	ItemQuantity(0)
{
}

//Modify Item Data
void USFItemBase::SetItemData(const FName& Name, const FSoftObjectPath& IconPath, FText Description,EItemType Type, int32 Quantity)
{
	ItemName = Name;
	ItemIcon = IconPath;
	ItemDescription = Description;
	ItemType = Type;
	ItemQuantity = Quantity;
}


//Initialize Item Data
void USFItemBase::InitializeItem(const USFItemBase& Item)
{
	ItemIcon = Item.ItemIcon;
	ItemName = Item.ItemName;
	ItemDescription = Item.ItemDescription;
	ItemType = Item.ItemType;
	ItemQuantity = Item.ItemQuantity;
}


EItemType USFItemBase::GetItemType() const
{
	return ItemType;
}