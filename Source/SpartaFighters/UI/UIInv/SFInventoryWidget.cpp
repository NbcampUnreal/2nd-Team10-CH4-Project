#include "UI/UIInv/SFInventoryWidget.h"
#include "Components/PanelWidget.h"
#include "Items/SFItemBase.h"
#include "Components/ScrollBox.h"
#include "Inventory/SFInventoryComponent.h"
#include "UI/UIInv/SFItemDisplayWidget.h"

void USFInventoryWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// InventoryItemListPanel valid check
	if (!InventoryItemListPanel)
	{
		UE_LOG(LogTemp, Warning, TEXT("InventoryItemListPanel is not binded to widget blueprint."));
		return;
	}

	// InventoryComponent
	if (InventoryComponent)
	{
		UpdateInventoryList();
	}
}

void USFInventoryWidget::InitializeInventoryWidget(USFInventoryComponent* InInventoryComponent)
{
	InventoryComponent = InInventoryComponent;
	if (InventoryComponent)
	{
		//Update UI delegate binding When Inv updated
		//InventoryComponent->OnRep_InventoryUpdated.AddUniqueDynamic(this, &USFInventoryWidget::UpdateInventoryList);
		UpdateInventoryList();
	}
}

void USFInventoryWidget::UpdateInventoryList()
{
	if (InventoryItemListPanel && InventoryComponent)
	{
	
		InventoryItemListPanel->ClearChildren();

		for (USFItemBase* Item : InventoryComponent->GetInventory())
		{
			if (Item)
			{
				USFItemDisplayWidget* ItemWidget = CreateWidget<USFItemDisplayWidget>(this, ItemDisplayWidgetClass);
				if (ItemWidget)
				{
					
					ItemWidget->SetItemInfo(Item);
					InventoryItemListPanel->AddChild(ItemWidget);
				}
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("InventoryItemListPanel, InventoryComponent or ItemDisplayWidgetClass is not valid."));
	}
}
