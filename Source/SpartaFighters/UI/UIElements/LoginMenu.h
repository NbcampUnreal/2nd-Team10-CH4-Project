#pragma once

#include "CoreMinimal.h"
#include "UI/BaseUserWidget.h"
#include "LoginMenu.generated.h"

class UEditableTextBox;
class UButton;
class UTextBlock;
class UAccountRegisterWidget;
class UQuitGameWidget;
class ULobbyMenu;
UCLASS()
class SPARTAFIGHTERS_API ULoginMenu : public UBaseUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;
    virtual void NativeDestruct() override;

    virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;

private:
	UPROPERTY(meta = (BindWidget))
	UEditableTextBox* IDTextBox;
	UPROPERTY(meta = (BindWidget))
	UEditableTextBox* PasswordTextBox;
    UPROPERTY(meta = (BindWidget))
    UButton* LoginButton;
    UPROPERTY(meta = (BindWidget))
    UButton* RegisterButton;
    UPROPERTY(meta = (BindWidget))
    UButton* OptionButton;
    UPROPERTY(meta = (BindWidget))
    UButton* QuitGameButton;
    UPROPERTY(meta = (BindWidget))
    UTextBlock* InstructionText;

    UFUNCTION()
    void ProcessLogin();
    UFUNCTION()
    void OnLoginClicked();
    UFUNCTION()
    void OnRegisterClicked();
    UFUNCTION()
    void OnOptionClicked();
    UFUNCTION()
    void OnQuitGameClicked();
    UFUNCTION()
	void OnTextCommitted(const FText& Text, ETextCommit::Type CommitMethod);

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
    TSubclassOf<UAccountRegisterWidget> AccountRegisterWidgetClass;
    UPROPERTY()
    UAccountRegisterWidget* AccountRegisterWidget;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
    TSubclassOf<UQuitGameWidget> QuitGameWidgetClass;
    UPROPERTY()
    UQuitGameWidget* QuitGameWidget;

private:
    UPROPERTY()
    FTimerHandle ResetInstructionTextTimerHandle;
    UPROPERTY()
    FTimerHandle EnterLobbyTimerHandle;

    void ResetInstructionText();
    void EnterLobby();

    void OnLogInSuccess();
    void OnLogInFailed(FString FailMessage);


};
