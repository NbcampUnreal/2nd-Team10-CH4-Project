#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Items/SFItemBase.h"
#include "SFItemDisplayWidget.generated.h"

UCLASS()
class SPARTAFIGHTERS_API USFItemDisplayWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UImage* ItemIconImage;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UTextBlock* ItemNameText;

	UFUNCTION(BlueprintCallable, Category = "Item Display")
	void SetItemInfo(class USFItemBase* Item);

protected:
	virtual void NativeConstruct() override;

	UPROPERTY(BlueprintReadOnly)
	class USFItemBase* CurrentItem;
};
