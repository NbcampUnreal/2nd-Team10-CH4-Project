

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SFItemBase.generated.h"

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
class SPARTAFIGHTERS_API USFItemBase : public UObject
{
	GENERATED_BODY()
	
public:
	USFItemBase();
	TSoftObjectPtr<UTexture2D> ItemIcon;
	FName ItemName;
	FText ItemDescription;
	EItemType Type;

	virtual void InitializeItem(const USFItemBase& Item);
	EItemType GetItemType() const;
	
};
