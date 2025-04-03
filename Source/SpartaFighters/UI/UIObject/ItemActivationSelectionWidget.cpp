#include "UI/UIObject/ItemActivationSelectionWidget.h"

void UItemActivationSelectionWidget::NativeConstruct()
{
	Super::NativeConstruct();
	Options = { TEXT("ON"), TEXT("OFF")};
	InitializeSelection(Options);
}