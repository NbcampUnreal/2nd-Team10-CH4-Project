#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/UIInv/SFItemDisplayWidget.h"
#include "SFInventoryWidget.generated.h"



UCLASS()
class SPARTAFIGHTERS_API USFInventoryWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UPanelWidget* InventoryItemListPanel; //panel to list items

	//View Item info
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	TSubclassOf<class USFItemDisplayWidget> ItemDisplayWidgetClass;

	//Set Inventory Component
	UFUNCTION(BlueprintCallable, Category = "Inventory UI")
	void InitializeInventoryWidget(class USFInventoryComponent* InventoryComponent);

protected:
	virtual void NativeConstruct() override;

public:
	class USFInventoryComponent* InventoryComponent;

	//Inventory Update
	UFUNCTION(BlueprintCallable, Category = "Inventory UI")
	void UpdateInventoryList();
};
