


#include "Items/SFItemBase.h"

USFItemBase::USFItemBase()
{
	Type = EItemType::Consumable;
}

void USFItemBase::InitializeItem(const USFItemBase& Item)
{

}

EItemType USFItemBase::GetItemType() const
{
	return Type;
}