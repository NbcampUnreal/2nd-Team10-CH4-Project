


#include "Items/SFItemBase.h"

USFItemBase::USFItemBase():
	ItemIcon(nullptr),
	ItemName(NAME_None),
	ItemDescription(FText::GetEmpty()),
	ItemType(EItemType::Common),
	Price(0)
{
}

//Modify Item Data
void USFItemBase::SetItemData(const FName& Name, TSoftObjectPtr<UTexture2D> IconPath, FText Description,EItemType Type)
{
	ItemName = Name;
	ItemIcon = IconPath;
	ItemDescription = Description;
	ItemType = Type;
}


//Initialize Item Data
void USFItemBase::InitializeItem(const USFItemBase& Item)
{
	ItemIcon = Item.ItemIcon;
	ItemName = Item.ItemName;
	ItemDescription = Item.ItemDescription;
	ItemType = Item.ItemType;
}


EItemType USFItemBase::GetItemType() const
{
	return ItemType;
}

void USFItemBase::OnItemAcquired_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("%s Item aquired."), *ItemName.ToString());
}

