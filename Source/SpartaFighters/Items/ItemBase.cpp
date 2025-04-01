


#include "Items/ItemBase.h"

UItemBase::UItemBase()
{
	Type = EItemType::Consumable;
}

void UItemBase::InitializeItem(const UItemBase& Item)
{

}

EItemType UItemBase::GetItemType() const
{
	return Type;
}