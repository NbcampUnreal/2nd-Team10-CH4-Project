
#include "CoreMinimal.h"
#include "Items/SFItemBase.h"
#include "Items/EquipItems/SFEquipableBase.h"
#include "FSFItemData.generated.h"

USTRUCT(BlueprintType)
struct FSFItemData : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item Data")
	FName ItemId;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item Data")
	FString ItemName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item Data", meta = (MultiLine = "true"))
	FString ItemDescription;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item Data")
	TSoftObjectPtr<UTexture2D> ItemIcon;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item Data")
	TSubclassOf<USFItemBase> ItemClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item Data")
	int32 Price = 0;

	
};
