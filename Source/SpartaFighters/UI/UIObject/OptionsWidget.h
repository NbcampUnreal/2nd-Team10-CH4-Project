#pragma once

#include "CoreMinimal.h"
#include "UI/BaseUserWidget.h"
#include "OptionsWidget.generated.h"

class USlider;
class UComboBoxString;
class UButton;
UCLASS()
class SPARTAFIGHTERS_API UOptionsWidget : public UBaseUserWidget
{
	GENERATED_BODY()
	
public:
    virtual void NativeConstruct() override;

protected:
    UPROPERTY(meta = (BindWidget))
    USlider* BGMVolumeSlider;
    UPROPERTY(meta = (BindWidget))
    USlider* EffectVolumeSlider;
    UPROPERTY(meta = (BindWidget))
    UComboBoxString* ResolutionComboBox;
    UPROPERTY(meta = (BindWidget))
    UComboBoxString* ScreenModeComboBox;
    UPROPERTY(meta = (BindWidget))
    UButton* ApplyButton;

    UFUNCTION()
    void OnBGMVolumeChanged(float NewValue);
    UFUNCTION()
    void OnEffectVolumeChanged(float NewValue);
    UFUNCTION()
    void OnResolutionSelected(FString SelectedItem, ESelectInfo::Type SelectionType);
    UFUNCTION()
    void OnScreenModeSelected(FString SelectedItem, ESelectInfo::Type SelectionType);

    void OnApplyButtonClicked();
};
