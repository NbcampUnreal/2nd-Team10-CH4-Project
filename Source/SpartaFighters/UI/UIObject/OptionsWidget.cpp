#include "UI/UIObject/OptionsWidget.h"

#include "Framework/OptionManager.h"
#include "GameFramework/GameUserSettings.h"
#include "Framework/SFGameInstance.h"
#include "Framework/SFGameInstanceSubsystem.h"
#include "UI/UIManager/UIManager.h"

#include "Components/Slider.h"
#include "Components/ComboBoxString.h"
#include "Components/Button.h"

#include "Kismet/GameplayStatics.h"

void UOptionsWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (BGMVolumeSlider)
	{
		float DefaultBGMVolume = 0.5f;
		BGMVolumeSlider->SetValue(DefaultBGMVolume);

		if (UOptionManager* OptionManager = GetGameInstance()->GetSubsystem<UOptionManager>())
		{
			OptionManager->BGSoundValue(DefaultBGMVolume);
			OptionManager->EFSoundValue(DefaultBGMVolume);
		}
	}

	if (BGMVolumeSlider)
	{
		BGMVolumeSlider->OnValueChanged.AddUniqueDynamic(this, &UOptionsWidget::OnBGMVolumeChanged);
	}

	if (EffectVolumeSlider)
	{
		EffectVolumeSlider->OnValueChanged.AddUniqueDynamic(this, &UOptionsWidget::OnEffectVolumeChanged);
	}

	if (ResolutionComboBox)
	{
		ResolutionComboBox->ClearOptions();
		ResolutionComboBox->AddOption("1920x1080");
		ResolutionComboBox->AddOption("1280x720");
		ResolutionComboBox->AddOption("2560x1440");
		ResolutionComboBox->OnSelectionChanged.AddUniqueDynamic(this, &UOptionsWidget::OnResolutionSelected);
	}

	if (ScreenModeComboBox)
	{
		ScreenModeComboBox->ClearOptions();
		ScreenModeComboBox->AddOption("FullScreen");
		ScreenModeComboBox->AddOption("WindowedFullScreen");
		ScreenModeComboBox->AddOption("Window");
		ScreenModeComboBox->OnSelectionChanged.AddUniqueDynamic(this, &UOptionsWidget::OnScreenModeSelected);
	}

	if (ApplyButton)
	{
		ApplyButton->OnClicked.AddUniqueDynamic(this, &UOptionsWidget::OnApplyButtonClicked);
	}
}

void UOptionsWidget::OnBGMVolumeChanged(float Value)
{
	UE_LOG(LogTemp, Warning, TEXT("OnBGMVolumeChanged"));
	if (UOptionManager* OptionManager = GetGameInstance()->GetSubsystem<UOptionManager>())
	{
		OptionManager->BGSoundValue(Value);
	}
}

void UOptionsWidget::OnEffectVolumeChanged(float Value)
{
	UE_LOG(LogTemp, Warning, TEXT("OnEffectVolumeChanged"));

	if (UOptionManager* OptionManager = GetGameInstance()->GetSubsystem<UOptionManager>())
	{
		OptionManager->EFSoundValue(Value);
	}
}

void UOptionsWidget::OnResolutionSelected(FString SelectedItem, ESelectInfo::Type SelectionType)
{
	UE_LOG(LogTemp, Warning, TEXT("OnResolutionSelected"));

	FString WidthStr, HeightStr;
	if (SelectedItem.Split(TEXT("x"), &WidthStr, &HeightStr))
	{
		int32 Width = FCString::Atoi(*WidthStr);
		int32 Height = FCString::Atoi(*HeightStr);
		if (UOptionManager* OptionManager = GetGameInstance()->GetSubsystem<UOptionManager>())
		{
			OptionManager->ChangeResolution(FIntPoint(Width, Height));
		}
	}
}

void UOptionsWidget::OnScreenModeSelected(FString SelectedItem, ESelectInfo::Type SelectionType)
{
	UE_LOG(LogTemp, Warning, TEXT("OnBGMVolumeChanged"));

	EScreenMode Mode = EScreenMode::FullScreen;
	if (SelectedItem == "FullScreen")
	{
		Mode = EScreenMode::FullScreen;
	}
	else if (SelectedItem == "WindowedFullScreen")
	{
		Mode = EScreenMode::WindowedFullScreen;
	}
	else if (SelectedItem == "Window")
	{
		Mode = EScreenMode::Window;
	}

	if (UOptionManager* OptionManager = GetGameInstance()->GetSubsystem<UOptionManager>())
	{
		OptionManager->ChangeScreen(Mode);
	}
}

void UOptionsWidget::OnApplyButtonClicked()
{
	UE_LOG(LogTemp, Warning, TEXT("OnApplyButtonClicked"));

	UGameUserSettings* Settings = GEngine->GetGameUserSettings();
	Settings->ApplySettings(false);

	if (USFGameInstance* GameInstance = Cast<USFGameInstance>(GetGameInstance()))
	{
		if (USFGameInstanceSubsystem* Subsystem = GameInstance->GetSubsystem<USFGameInstanceSubsystem>())
		{
			if (UUIManager* UIManager = ResolveUIManager())
			{
				UIManager->CloseOptionsWidget();
			}
		}
	}
}
