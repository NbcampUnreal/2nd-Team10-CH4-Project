

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/Texture2D.h"
#include "SFItemBase.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class EItemType:uint8
{
	Common=0 UMETA(DisplayName="Common"),
	Exclusive UMETA(DisplayName = "Exclusive"),
	Cosmetic UMETA(DisplayName = "Cosmetic"),
	Consumable UMETA(DisplayName = "Consumable")
};

UCLASS(BlueprintType)
class SPARTAFIGHTERS_API USFItemBase : public UObject
{
	GENERATED_BODY()
	
public:
	USFItemBase();
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item")
	TSoftObjectPtr<UTexture2D> ItemIcon;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item")
	FName ItemName;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item")
	FText ItemDescription;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item")
	EItemType ItemType;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item")
	int32 ItemQuantity;


	UFUNCTION(BlueprintCallable, Category = "Item")
	void SetItemData(const FName& Name, const FSoftObjectPath& IconPath, FText Description, EItemType Type);

	//implement on blueprint
	UFUNCTION(BlueprintImplementableEvent)
	UTexture2D* GetItemIcon() const;
	EItemType GetItemType() const;

	virtual void InitializeItem(const USFItemBase& Item);

	//Event called when item aquired
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Item")
	void OnItemAcquired();
	virtual void OnItemAcquired_Implementation();
	
};
