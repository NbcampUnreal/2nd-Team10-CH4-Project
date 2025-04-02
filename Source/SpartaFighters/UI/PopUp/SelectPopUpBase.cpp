#include "UI/PopUp/SelectPopUpBase.h"
#include "Components/Button.h"

void USelectPopUpBase::NativeConstruct()
{
	Super::NativeConstruct();
	if (ExitButton)
	{
		ExitButton->OnClicked.AddDynamic(this, &USelectPopUpBase::OnExitClicked);
	}
}

void USelectPopUpBase::OnExitClicked()
{
    SetVisibility(ESlateVisibility::Hidden);
}