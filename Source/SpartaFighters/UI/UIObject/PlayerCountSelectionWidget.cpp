#include "UI/UIObject/PlayerCountSelectionWidget.h"

void UPlayerCountSelectionWidget::NativeConstruct()
{
	Super::NativeConstruct();
	Options = { TEXT("1"), TEXT("2"), TEXT("3"), TEXT("4") };
	InitializeSelection(Options);
}