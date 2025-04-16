#include "QuitGameWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Kismet/KismetSystemLibrary.h"

#define LOCTEXT_NAMESPACE "SpartaFighters"

void UQuitGameWidget::NativeConstruct()
{
	Super::NativeConstruct();
	if (QuitGameButton)
	{
		QuitGameButton->OnClicked.AddUniqueDynamic(this, &UQuitGameWidget::OnQuitGameClicked);
	}
	if (QuitGameText)
	{
		QuitGameText->SetText(LOCTEXT("QuitGameTextText", "Really Want to Quit Game?"));
	}
}

void UQuitGameWidget::NativeDestruct()
{
	Super::NativeDestruct();
	if (QuitGameButton)
	{
		QuitGameButton->OnClicked.RemoveDynamic(this, &UQuitGameWidget::OnQuitGameClicked);
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