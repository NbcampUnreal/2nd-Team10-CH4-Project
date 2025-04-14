#pragma once

#include "CoreMinimal.h"
#include "UI/BaseUserWidget.h"
#include "SelectPopUpBase.generated.h"

class UButton;
UCLASS()
class SPARTAFIGHTERS_API USelectPopUpBase : public UBaseUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	UPROPERTY(meta = (BindWidget))
	UButton* ExitButton;
	
	UFUNCTION()
	virtual void OnExitClicked();
};
