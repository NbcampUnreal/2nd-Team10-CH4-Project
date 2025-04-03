#pragma once

#include "CoreMinimal.h"
#include "UI/UIObject/SelectionWidget.h"
#include "ItemActivationSelectionWidget.generated.h"

UCLASS()
class SPARTAFIGHTERS_API UItemActivationSelectionWidget : public USelectionWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;
};
