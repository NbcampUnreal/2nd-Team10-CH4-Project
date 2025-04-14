#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CharacterStatusWidget.generated.h"

class UTextBlock;
class UProgressBar;
class UImage;

UCLASS()
class SPARTAFIGHTERS_API UCharacterStatusWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* PlayerName;
    UPROPERTY(meta = (BindWidget))
    UProgressBar* KnockBackBar;
    UPROPERTY(meta = (BindWidget))
    UProgressBar* HPBar;
    UPROPERTY(meta = (BindWidget))
    UProgressBar* MPBar;
    UPROPERTY(meta = (BindWidget))
    UImage* CharacterIcon;
};
