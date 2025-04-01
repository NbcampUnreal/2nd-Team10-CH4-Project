#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LoginMenu.generated.h"

class UEditableTextBox;
class UButton;
class UTextBlock;
class UAccountRegisterWidget;
UCLASS()
class SPARTAFIGHTERS_API ULoginMenu : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;
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
    UButton* ExitButton;
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
    void OnExitClicked();
    UFUNCTION()
	void OnTextCommitted(const FText& Text, ETextCommit::Type CommitMethod);

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
    TSubclassOf<UAccountRegisterWidget> AccountRegisterWidgetClass;
    UPROPERTY()
    UAccountRegisterWidget* AccountRegisterWidget;

private:
    UPROPERTY()
    FTimerHandle ResetInstructionTextTimerHandle;

    void ResetInstructionText();


};
