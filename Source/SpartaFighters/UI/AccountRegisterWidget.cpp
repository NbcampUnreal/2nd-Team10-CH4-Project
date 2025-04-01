#include "AccountRegisterWidget.h"
#include "Components/EditableTextBox.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"

#define LOCTEXT_NAMESPACE "SpartaFighters"

void UAccountRegisterWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (RegisterConfirmButton)
    {
        RegisterConfirmButton->OnClicked.AddDynamic(this, &UAccountRegisterWidget::OnRegisterConfirmClicked);
    }
    if (RegisterCancelButton)
    {
        RegisterCancelButton->OnClicked.AddDynamic(this, &UAccountRegisterWidget::OnRegisterCancelClicked);
    }
    if (RegisterInstructionText)
    {
        RegisterInstructionText->SetText(LOCTEXT("DefaultInstructionText", "Welcome To Sparta Fighters!"));
    }
}

void UAccountRegisterWidget::OnRegisterConfirmClicked()
{
    if (NewIDTextBox && NewPasswordTextBox && ConfirmPasswordTextBox)
    {
        FString NewID = NewIDTextBox->GetText().ToString();
        FString NewPassword = NewPasswordTextBox->GetText().ToString();
        FString ConfirmPassword = ConfirmPasswordTextBox->GetText().ToString();

        if (NewID.IsEmpty() || NewPassword.IsEmpty() || ConfirmPassword.IsEmpty())
        {
            RegisterInstructionText->SetText(FText::FromString("Please fill in all fields."));
            GetWorld()->GetTimerManager().SetTimer(
                ResetRegisterInstructionTextTimerHandle,
                this,
                &UAccountRegisterWidget::ResetRegisterInstructionText,
                3.0f,
                false);
            return;
        }

        if (NewPassword != ConfirmPassword)
        {
            RegisterInstructionText->SetText(FText::FromString("Password doesn't match."));
            GetWorld()->GetTimerManager().SetTimer(
                ResetRegisterInstructionTextTimerHandle,
                this,
                &UAccountRegisterWidget::ResetRegisterInstructionText,
                3.0f,
                false);
            return;
        }

        UE_LOG(LogTemp, Log, TEXT("Account Registered: %s"), *NewID);
        ProcessRegisterSuccess();
    }
}

void UAccountRegisterWidget::ProcessRegisterSuccess()
{
    RegisterInstructionText->SetText(FText::FromString("Sign up successfully!"));
    RegisterConfirmButton->SetIsEnabled(false);
    GetWorld()->GetTimerManager().SetTimer(
        RegisterButtonTimerHandle,
        this,
        &UAccountRegisterWidget::OnRegisterCancelClicked,
        1.0f,
        false);
}

void UAccountRegisterWidget::ResetRegisterWidget()
{
    UE_LOG(LogTemp, Warning, TEXT("ResetRegisterWidget Called"));
    if (RegisterInstructionText)
    {
        RegisterInstructionText->SetText(LOCTEXT("DefaultInstructionText", "Welcome To Sparta Fighters!"));
    }
	if (NewIDTextBox)
	{
		NewIDTextBox->SetText(FText::GetEmpty());
	}
	if (NewPasswordTextBox)
	{
		NewPasswordTextBox->SetText(FText::GetEmpty());
	}
	if (ConfirmPasswordTextBox)
	{
		ConfirmPasswordTextBox->SetText(FText::GetEmpty());
	}
	RegisterConfirmButton->SetIsEnabled(true);
}

void UAccountRegisterWidget::OnRegisterCancelClicked()
{
    SetVisibility(ESlateVisibility::Hidden);
}

void UAccountRegisterWidget::ResetRegisterInstructionText()
{
    if (RegisterInstructionText)
    {
        RegisterInstructionText->SetText(LOCTEXT("DefaultInstructionText", "Welcome To Sparta Fighters!"));
    }
}

#undef LOCTEXT_NAMESPACE