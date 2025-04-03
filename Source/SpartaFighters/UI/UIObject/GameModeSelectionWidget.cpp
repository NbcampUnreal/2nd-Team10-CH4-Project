#include "UI/UIObject/GameModeSelectionWidget.h"

void UGameModeSelectionWidget::NativeConstruct()
{
	Super::NativeConstruct();
	Options = { TEXT("Cooperative"), TEXT("Battle"), TEXT("Single") };
	InitializeSelection(Options);
}