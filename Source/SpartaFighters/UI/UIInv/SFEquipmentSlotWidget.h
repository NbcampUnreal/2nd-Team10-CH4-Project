#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Items/EquipItems/SFEquipableBase.h"
#include "Inventory/SFInventoryComponent.h"
#include "SFEquipmentSlotWidget.generated.h"

//DECLARE_DELEGATE_OneParam(FOnUnequipItemSignature, SFEquipSlot);

UCLASS()
class SPARTAFIGHTERS_API USFEquipmentSlotWidget : public UUserWidget
{
	GENERATED_BODY()

//public:
//	// Sets default values for this component's properties
//	USFEquipmentSlotWidget();
//
//protected:
//	virtual void NativeConstruct() override;
//
//public:
//	//Set Equipped item
//	UFUNCTION(BlueprintCallable, Category = "Equipment Slot Widget")
//	void SetEquippedItem(const USFEquipableBase* Item);
//
//	
//	UFUNCTION(BlueprintCallable, Category = "Equipment Slot Widget")
//	void SetSlotType(SFEquipSlot SlotType);
//
//	
//	//UFUNCTION(BlueprintCallable, Category = "Equipment Slot Widget")
//	//void SetUnequipDelegate(FOnUnequipItemSignature Delegate);
//
//	//
//	//UFUNCTION()
//	//void NativeOnUnequipButtonClicked();
//
//protected:
//
//	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
//	UImage* EquipItemIcon;
//
//
//	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
//	UTextBlock* EquipSlotName;
//
//
//	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
//	UButton* UnequipButton;
//
//	// 
//	const USFEquipableBase* CurrentEquippedItem;
//
//	// 
//	SFEquipSlot CurrentSlotType;
//
//	// 
//	//FOnUnequipItemSignature OnUnequipButtonClickedDelegate;
//	
};
