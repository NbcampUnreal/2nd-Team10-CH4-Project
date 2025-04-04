#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WebAPIClient/ResponseBody/ResultUser.h"
#include "PlayerSimpleInfoWidget.generated.h"

class UTextBlock;
class UProgressBar;

UCLASS()
class SPARTAFIGHTERS_API UPlayerSimpleInfoWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* PlayerIDText;
    UPROPERTY(meta = (BindWidget))
    UProgressBar* ExperienceBar;
    UPROPERTY(meta = (BindWidget))
    UTextBlock* ExperienceText;

public:
    // TODO : Add RequiredExperience, CurrentExperience to UserData
    void SetPlayerInfo(const FUser& UserData);
};
