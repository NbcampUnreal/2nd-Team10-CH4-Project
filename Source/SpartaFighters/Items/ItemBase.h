

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ItemBase.generated.h"

/**
 * 
 */
enum class EItemType
{
	Common = 0,
	Exclusive,
	Cosmetic,
	Consumable
};

UCLASS()
class SPARTAFIGHTERS_API UItemBase : public UObject
{
	GENERATED_BODY()
	
public:
	UItemBase();
	TSoftObjectPtr<UTexture2D> ItemIcon;
	FName ItemName;
	FText ItemDescription;
	EItemType Type;

	virtual void InitializeItem(const UItemBase& Item);
	EItemType GetItemType() const;
	
};
