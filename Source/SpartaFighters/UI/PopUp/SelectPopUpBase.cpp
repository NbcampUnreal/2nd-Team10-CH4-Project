#include "UI/PopUp/SelectPopUpBase.h"
#include "Components/Button.h"

void USelectPopUpBase::NativeConstruct()
{
	Super::NativeConstruct();
	if (ExitButton)
	{
		ExitButton->OnClicked.AddUniqueDynamic(this, &USelectPopUpBase::OnExitClicked);
	}
}

void USelectPopUpBase::NativeDestruct()
{
	Super::NativeDestruct();
	if (ExitButton)
	{
		ExitButton->OnClicked.RemoveDynamic(this, &USelectPopUpBase::OnExitClicked);
	}
}

void USelectPopUpBase::OnExitClicked()
{
    SetVisibility(ESlateVisibility::Hidden);
}