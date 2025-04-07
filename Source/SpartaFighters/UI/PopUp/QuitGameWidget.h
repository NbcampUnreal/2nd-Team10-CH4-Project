#pragma once

#include "CoreMinimal.h"
#include "UI/PopUp/SelectPopUpBase.h"
#include "QuitGameWidget.generated.h"

class UTextBlock;
UCLASS()
class SPARTAFIGHTERS_API UQuitGameWidget : public USelectPopUpBase
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

private:
	UPROPERTY(meta = (BindWidget))
	UButton* QuitGameButton;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* QuitGameText;

	UFUNCTION()
	void OnQuitGameClicked();
	
};
