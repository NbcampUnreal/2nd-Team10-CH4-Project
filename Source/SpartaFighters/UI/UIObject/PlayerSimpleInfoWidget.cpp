#include "UI/UIObject/PlayerSimpleInfoWidget.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"

void UPlayerSimpleInfoWidget::NativeConstruct()
{
    Super::NativeConstruct();
}

void UPlayerSimpleInfoWidget::SetPlayerInfo(const FUser& UserData)
{
    /*PlayerIDText->SetText(FText::FromString(UserData.PlayerID));

    float ExpRatio = UserData.RequiredExperience > 0 ?
        static_cast<float>(UserData.CurrentExperience) / UserData.RequiredExperience : 0.f;

    ExperienceBar->SetPercent(ExpRatio);

    FString ExpText = FString::Printf(TEXT("%d / %d"), UserData.CurrentExperience, UserData.RequiredExperience);
    ExperienceText->SetText(FText::FromString(ExpText));*/

	// TODO : Add RequiredExperience, CurrentExperience to UserData
    ensureAlways(false);
}
