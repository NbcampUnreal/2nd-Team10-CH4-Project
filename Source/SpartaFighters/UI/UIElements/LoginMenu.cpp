#include "LoginMenu.h"
#include "Framework/SFGameInstance.h"
#include "Framework/SFGameInstanceSubsystem.h"
#include "Framework/SFLobbyPlayerController.h"
#include "Framework/SFPlayerState.h"
#include "Framework/SFLoginPlayerController.h"

#include "Components/EditableTextBox.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"

#include "UI/PopUp/AccountRegisterWidget.h"
#include "UI/PopUp/QuitGameWidget.h"
#include "UI/UIManager/UIManager.h"

#include "Common/DefaultGameIni.h"
#include "WebAPIClient/Controller/UserController.h"


#define LOCTEXT_NAMESPACE "SpartaFighters"

void ULoginMenu::NativeConstruct()
{
	Super::NativeConstruct();

	if (LoginButton)
	{
		LoginButton->OnClicked.AddUniqueDynamic(this, &ULoginMenu::OnLoginClicked);
	}
	if (RegisterButton)
	{
		RegisterButton->OnClicked.AddUniqueDynamic(this, &ULoginMenu::OnRegisterClicked);
	}
	if (OptionButton)
	{
		OptionButton->OnClicked.AddUniqueDynamic(this, &ULoginMenu::OnOptionClicked);
	}
	if (QuitGameButton)
	{
		QuitGameButton->OnClicked.AddUniqueDynamic(this, &ULoginMenu::OnQuitGameClicked);
	}
	if (IDTextBox)
	{
		IDTextBox->OnTextCommitted.AddUniqueDynamic(this, &ULoginMenu::OnTextCommitted);
	}
	if (PasswordTextBox)
	{
		PasswordTextBox->OnTextCommitted.AddUniqueDynamic(this, &ULoginMenu::OnTextCommitted);
	}
	if (InstructionText)
	{
		InstructionText->SetText(LOCTEXT("DefaultInstructionText", "Welcome To Sparta Fighters!"));
	}

	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		FInputModeUIOnly InputModeData;
		PlayerController->SetInputMode(InputModeData);
		PlayerController->bShowMouseCursor = true;
	}
}

void ULoginMenu::NativeDestruct()
{
	Super::NativeDestruct();

	if (LoginButton)
	{
		LoginButton->OnClicked.RemoveDynamic(this, &ULoginMenu::OnLoginClicked);
	}
	if (RegisterButton)
	{
		RegisterButton->OnClicked.RemoveDynamic(this, &ULoginMenu::OnRegisterClicked);
	}
	if (OptionButton)
	{
		OptionButton->OnClicked.RemoveDynamic(this, &ULoginMenu::OnOptionClicked);
	}
	if (QuitGameButton)
	{
		QuitGameButton->OnClicked.RemoveDynamic(this, &ULoginMenu::OnQuitGameClicked);
	}
	if (IDTextBox)
	{
		IDTextBox->OnTextCommitted.RemoveDynamic(this, &ULoginMenu::OnTextCommitted);
	}
	if (PasswordTextBox)
	{
		PasswordTextBox->OnTextCommitted.RemoveDynamic(this, &ULoginMenu::OnTextCommitted);
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

		if (UDefaultGameIni::GetInstance()->GetWebAPIUse())
		{
			FRequestLogIn Request;
			Request.Account = ID;
			Request.Password = Password;

			UUserController::GetInstance()->LogIn(Request,
				[this](TSharedPtr<FResultLogIn> Response)
				{
					if (Response.IsValid())
					{
						EResultCode ResultCode = static_cast<EResultCode>(Response->ResultCode);
						switch (ResultCode)
						{
						case EResultCode::OK:
							OnLogInSuccess();
							break;
							//case EResultCode::AccountNotFound:
							//	OnLogInFailed("Fail : Account Not Found!");
							//	break;
							//case EResultCode::PasswordMismatched:
							//	OnLogInFailed("Fail : PasswordMismatched!");
							//	break;
							//case EResultCode::AlreadyLogIn:
							//	OnLogInFailed("Fail : AlreadyLogIn!");
							//	break;
						default:
							OnLogInFailed(FString::Printf(TEXT("Fail[%d] : %s"), Response->ResultCode, *Response->ResultMessage));
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
			if (ID == "TestUser" && Password == "1234")
			{
				OnLogInSuccess();
			}
			else
			{
				OnLogInFailed("LoginError : Invalid ID or Password. Please Check and Try Again.");
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
		UWorld* World = GetWorld();
		if (!World)
		{
			return;
		}

		AccountRegisterWidget = CreateWidget<UAccountRegisterWidget>(World, AccountRegisterWidgetClass);
		if (AccountRegisterWidget)
		{
			AccountRegisterWidget->AddToViewport();
		}
	}

	if (AccountRegisterWidget)
	{
		AccountRegisterWidget->ResetRegisterWidget();
		AccountRegisterWidget->SetVisibility(ESlateVisibility::Visible);

		if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
		{
			FInputModeUIOnly InputModeData;
			PlayerController->SetInputMode(InputModeData);
			PlayerController->bShowMouseCursor = true;
		}
	}
}

void ULoginMenu::OnOptionClicked()
{
	UE_LOG(LogTemp, Log, TEXT("Option Button Clicked"));
	UE_LOG(LogTemp, Log, TEXT("OnOptionButtonClicked"));
	if (USFGameInstance* GameInstance = Cast<USFGameInstance>(GetGameInstance()))
	{
		if (USFGameInstanceSubsystem* Subsystem = GameInstance->GetSubsystem<USFGameInstanceSubsystem>())
		{
			if (UUIManager* UIManager = Subsystem->GetUIManager())
			{
				UIManager->ShowOptionsWidget();
			}
		}
	}
}

void ULoginMenu::OnQuitGameClicked()
{
	if (!QuitGameWidget && QuitGameWidgetClass)
	{
		UWorld* World = GetWorld();
		if (!World)
		{
			return;
		}

		QuitGameWidget = CreateWidget<UQuitGameWidget>(World, QuitGameWidgetClass);
		if (QuitGameWidget)
		{
			QuitGameWidget->AddToViewport();
		}
	}

	if (QuitGameWidget)
	{
		QuitGameWidget->SetVisibility(ESlateVisibility::Visible);

		if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
		{
			FInputModeUIOnly InputModeData;
			PlayerController->SetInputMode(InputModeData);
			PlayerController->bShowMouseCursor = true;
		}
	}
}

void ULoginMenu::ResetInstructionText()
{
	if (InstructionText)
	{
		InstructionText->SetText(LOCTEXT("DefaultInstructionText", "Welcome To Sparta Fighters!"));
	}
}

void ULoginMenu::EnterLobby()
{
	
	/*if (USFGameInstance* GameInstance = Cast<USFGameInstance>(GetGameInstance()))
	{
		if (USFGameInstanceSubsystem* Subsystem = GameInstance->GetSubsystem<USFGameInstanceSubsystem>())
		{
			const FString LobbyMapName = TEXT("LobbyMenu");
			Subsystem->ChangeLevelByMapName(LobbyMapName);
		}
	}*/
	
	ASFLoginPlayerController* LoginPlayerController = Cast<ASFLoginPlayerController>(GetWorld()->GetFirstPlayerController());
	if (LoginPlayerController != nullptr)
	{
		LoginPlayerController->Client_TravelToLobby();
	}
}

void ULoginMenu::OnLogInSuccess()
{
	UE_LOG(LogTemp, Log, TEXT("Login Successful"));

	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (PlayerController)
	{
		ASFPlayerState* SFPlayerState = PlayerController->GetPlayerState<ASFPlayerState>();
		if (SFPlayerState)
		{
			FString ID = IDTextBox->GetText().ToString();
			SFPlayerState->SetUniqueID(ID);
			UE_LOG(LogTemp, Warning, TEXT("[SFPlayerState->PlayerUniqueID : %s]"), *SFPlayerState->GetUniqueID());
		}
	}

	if (InstructionText)
	{
		InstructionText->SetText(LOCTEXT("LoginSuccess", "Login Success! Start the game."));
		GetWorld()->GetTimerManager().SetTimer(
			EnterLobbyTimerHandle,
			this,
			&ULoginMenu::EnterLobby,
			1.0f,
			false);
	}
}

void ULoginMenu::OnLogInFailed(FString FailMessage)
{
	UE_LOG(LogTemp, Warning, TEXT("Login Failed"));
	IDTextBox->SetText(FText::GetEmpty());
	PasswordTextBox->SetText(FText::GetEmpty());
	if (InstructionText)
	{
		InstructionText->SetText(FText::FromString(FailMessage));
		//InstructionText->SetText(LOCTEXT("LoginError", "Invalid ID or Password. Please Check and Try Again."));
		GetWorld()->GetTimerManager().SetTimer(
			ResetInstructionTextTimerHandle,
			this,
			&ULoginMenu::ResetInstructionText,
			3.0f,
			false);
	}
}

#undef LOCTEXT_NAMESPACE