#pragma once

#include "CoreMinimal.h"
#include "UI/PopUp/SelectPopUpBase.h"
#include "AccountRegisterWidget.generated.h"

class UEditableTextBox;
class UButton;
class UTextBlock;
UCLASS()
class SPARTAFIGHTERS_API UAccountRegisterWidget : public USelectPopUpBase
{
	GENERATED_BODY()
	
protected:
    virtual void NativeConstruct() override;

private:
    UPROPERTY(meta = (BindWidget))
    UEditableTextBox* NewIDTextBox;
    UPROPERTY(meta = (BindWidget))
    UEditableTextBox* NewPasswordTextBox;
    UPROPERTY(meta = (BindWidget))
    UEditableTextBox* ConfirmPasswordTextBox;
    UPROPERTY(meta = (BindWidget))
    UButton* RegisterConfirmButton;
    UPROPERTY(meta = (BindWidget))
    UTextBlock* RegisterInstructionText;

    UFUNCTION()
    void OnRegisterConfirmClicked();
    UFUNCTION()
    void ProcessRegisterSuccess();

public:
    UFUNCTION()
    void ResetRegisterWidget();

private:
    UPROPERTY()
    FTimerHandle ResetRegisterInstructionTextTimerHandle;
    UPROPERTY()
    FTimerHandle RegisterButtonTimerHandle;
    
    void ResetRegisterInstructionText();
};
