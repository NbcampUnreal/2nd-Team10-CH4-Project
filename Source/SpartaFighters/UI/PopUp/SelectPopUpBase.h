#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SelectPopUpBase.generated.h"

class UButton;
UCLASS()
class SPARTAFIGHTERS_API USelectPopUpBase : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
	UButton* ExitButton;
	
	UFUNCTION()
	virtual void OnExitClicked();
};
