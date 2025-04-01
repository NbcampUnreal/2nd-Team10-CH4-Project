#include "UI/QuitGameWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Kismet/KismetSystemLibrary.h"

#define LOCTEXT_NAMESPACE "SpartaFighters"


void UQuitGameWidget::NativeConstruct()
{
	Super::NativeConstruct();
	if (QuitGameButton)
	{
		QuitGameButton->OnClicked.AddDynamic(this, &UQuitGameWidget::OnQuitGameClicked);
	}
	if (QuitGameText)
	{
		QuitGameText->SetText(LOCTEXT("QuitGameTextText", "Really Want to Quit Game?"));
	}
}

void UQuitGameWidget::OnQuitGameClicked()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), nullptr, EQuitPreference::Quit, false);
}

#undef LOCTEXT_NAMESPACE