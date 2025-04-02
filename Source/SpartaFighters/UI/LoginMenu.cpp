#include "UI/LoginMenu.h"
#include "Framework/SFPlayerController.h"
#include "Components/EditableTextBox.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "UI/AccountRegisterWidget.h"
#include "UI/QuitGameWidget.h"

#define LOCTEXT_NAMESPACE "SpartaFighters"

void ULoginMenu::NativeConstruct()
{
	Super::NativeConstruct();

	if (LoginButton)
	{
		LoginButton->OnClicked.AddDynamic(this, &ULoginMenu::OnLoginClicked);
	}
	if (RegisterButton)
	{
		RegisterButton->OnClicked.AddDynamic(this, &ULoginMenu::OnRegisterClicked);
	}
	if (OptionButton)
	{
		OptionButton->OnClicked.AddDynamic(this, &ULoginMenu::OnOptionClicked);
	}
	if (QuitGameButton)
	{
		QuitGameButton->OnClicked.AddDynamic(this, &ULoginMenu::OnQuitGameClicked);
	}
	if (IDTextBox)
	{
		IDTextBox->OnTextCommitted.AddDynamic(this, &ULoginMenu::OnTextCommitted);
	}
	if (PasswordTextBox)
	{
		PasswordTextBox->OnTextCommitted.AddDynamic(this, &ULoginMenu::OnTextCommitted);
	}
	if (InstructionText)
	{
		InstructionText->SetText(LOCTEXT("DefaultInstructionText", "Welcome To Sparta Fighters!"));
	}
}

// DB Connection : Login processing logic requires Connection with DB
void ULoginMenu::ProcessLogin()
{
	if (IDTextBox && PasswordTextBox)
	{
		FString ID = IDTextBox->GetText().ToString();
		FString Password = PasswordTextBox->GetText().ToString();

		if (ID == "TestUser" && Password == "1234")
		{
			UE_LOG(LogTemp, Log, TEXT("Login Successful"));
			if (InstructionText)
			{
				InstructionText->SetText(LOCTEXT("LoginSuccess", "Login Success! Start the game."));
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Login Failed"));
			IDTextBox->SetText(FText::GetEmpty());
			PasswordTextBox->SetText(FText::GetEmpty());
			if (InstructionText)
			{
				InstructionText->SetText(LOCTEXT("LoginError", "Invalid ID or Password. Please Check and Try Again."));
				GetWorld()->GetTimerManager().SetTimer(
					ResetInstructionTextTimerHandle,
					this,
					&ULoginMenu::ResetInstructionText,
					3.0f,
					false);
			}
		}
	}
}

void ULoginMenu::OnLoginClicked()
{
	ProcessLogin();
}

void ULoginMenu::OnTextCommitted(const FText& Text, ETextCommit::Type CommitMethod)
{
	if (CommitMethod == ETextCommit::OnEnter)
	{
		if (IDTextBox && PasswordTextBox)
		{
			ProcessLogin();
		}
	}
}

FReply ULoginMenu::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	if (InKeyEvent.GetKey() == EKeys::Tab)
	{
		if (IDTextBox && IDTextBox->HasKeyboardFocus() && PasswordTextBox)
		{
			PasswordTextBox->SetKeyboardFocus();
			return FReply::Handled();
		}
	}
	return Super::NativeOnKeyDown(InGeometry, InKeyEvent);
}

void ULoginMenu::OnRegisterClicked()
{
	if (!AccountRegisterWidget && AccountRegisterWidgetClass)
	{
		AccountRegisterWidget = CreateWidget<UAccountRegisterWidget>(GetWorld(), AccountRegisterWidgetClass);

		if (AccountRegisterWidget)
		{
			AccountRegisterWidget->AddToViewport();
			if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
			{
				FInputModeUIOnly InputModeData;
				PlayerController->SetInputMode(InputModeData);
				PlayerController->bShowMouseCursor = true;
			}
		}
	}
	else
	{
		AccountRegisterWidget->SetVisibility(ESlateVisibility::Visible);
		AccountRegisterWidget->ResetRegisterWidget();
	}
}

void ULoginMenu::OnOptionClicked()
{
	UE_LOG(LogTemp, Log, TEXT("Option Button Clicked"));
}

void ULoginMenu::OnQuitGameClicked()
{
	if (!QuitGameWidget && QuitGameWidgetClass)
	{
		QuitGameWidget = CreateWidget<UQuitGameWidget>(GetWorld(), QuitGameWidgetClass);
		if (QuitGameWidget)
		{
			QuitGameWidget->AddToViewport();
			if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
			{
				FInputModeUIOnly InputModeData;
				PlayerController->SetInputMode(InputModeData);
				PlayerController->bShowMouseCursor = true;
			}
		}
	}
	else
	{
		QuitGameWidget->SetVisibility(ESlateVisibility::Visible);
	}
}

void ULoginMenu::ResetInstructionText()
{
	if (InstructionText)
	{
		InstructionText->SetText(LOCTEXT("DefaultInstructionText", "Welcome To Sparta Fighters!"));
	}
}

#undef LOCTEXT_NAMESPACE