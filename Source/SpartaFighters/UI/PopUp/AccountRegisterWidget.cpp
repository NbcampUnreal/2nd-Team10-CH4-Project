#include "AccountRegisterWidget.h"
#include "Components/EditableTextBox.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"

#include "Common/DefaultGameIni.h"
#include "WebAPIClient/Controller/UserController.h"

#define LOCTEXT_NAMESPACE "SpartaFighters"

void UAccountRegisterWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (RegisterConfirmButton)
    {
        RegisterConfirmButton->OnClicked.AddUniqueDynamic(this, &UAccountRegisterWidget::OnRegisterConfirmClicked);
    }
    if (RegisterInstructionText)
    {
        RegisterInstructionText->SetText(LOCTEXT("DefaultInstructionText", "Welcome To Sparta Fighters!"));
    }
}

void UAccountRegisterWidget::NativeDestruct()
{
    Super::NativeDestruct();

    if (RegisterConfirmButton)
    {
        RegisterConfirmButton->OnClicked.RemoveDynamic(this, &UAccountRegisterWidget::OnRegisterConfirmClicked);
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
            OnRegisterFailed("Please fill in all fields.");
            //RegisterInstructionText->SetText(FText::FromString("Please fill in all fields."));
            //GetWorld()->GetTimerManager().SetTimer(
            //    ResetRegisterInstructionTextTimerHandle,
            //    this,
            //    &UAccountRegisterWidget::ResetRegisterInstructionText,
            //    3.0f,
            //    false);
            return;
        }

        if (NewPassword != ConfirmPassword)
        {
            OnRegisterFailed("Password doesn't match.");
            //RegisterInstructionText->SetText(FText::FromString("Password doesn't match."));
            //GetWorld()->GetTimerManager().SetTimer(
            //    ResetRegisterInstructionTextTimerHandle,
            //    this,
            //    &UAccountRegisterWidget::ResetRegisterInstructionText,
            //    3.0f,
            //    false);
            return;
        }

        UE_LOG(LogTemp, Log, TEXT("Account Registered: %s"), *NewID);

        if (UDefaultGameIni::GetInstance()->GetWebAPIUse())
        {
            FRequestCreateAccount Request;
            Request.Account = NewID;
            Request.Password = NewPassword;

            UUserController::GetInstance()->CreateAccount(Request,
                [this](TSharedPtr<FResultCreateAccount> Response)
                {
                    if (Response.IsValid())
                    {
                        EResultCode ResultCode = static_cast<EResultCode>(Response->ResultCode);
                        switch (ResultCode)
                        {
                        case EResultCode::OK:
                            ProcessRegisterSuccess();
                            break;
                        default:
                            OnRegisterFailed(FString::Printf(TEXT("Fail[%d] : %s"), Response->ResultCode, *Response->ResultMessage));
                            break;
                        }
                    }
                    else
                    {
                        UE_LOG(LogTemp, Error, TEXT("Invalid response or network error occurred"));
                    }
                });
        }
        else
        {
            ProcessRegisterSuccess();
        }
    }
}

// DB Connection : Register Confirm processing logic requires Connection with DB
void UAccountRegisterWidget::ProcessRegisterSuccess()
{
    RegisterInstructionText->SetText(FText::FromString("Sign up successfully!"));
    RegisterConfirmButton->SetIsEnabled(false);
    GetWorld()->GetTimerManager().SetTimer(
        RegisterButtonTimerHandle,
        this,
        &UAccountRegisterWidget::OnExitClicked,
        1.0f,
        false);
}

void UAccountRegisterWidget::OnRegisterFailed(FString FailMessage)
{
    RegisterInstructionText->SetText(FText::FromString(FailMessage));
    GetWorld()->GetTimerManager().SetTimer(
        ResetRegisterInstructionTextTimerHandle,
        this,
        &UAccountRegisterWidget::ResetRegisterInstructionText,
        3.0f,
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

void UAccountRegisterWidget::ResetRegisterInstructionText()
{
    if (RegisterInstructionText)
    {
        RegisterInstructionText->SetText(LOCTEXT("DefaultInstructionText", "Welcome To Sparta Fighters!"));
    }
}

#undef LOCTEXT_NAMESPACE