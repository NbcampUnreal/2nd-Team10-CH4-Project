#pragma once

#include "CoreMinimal.h"
#include "UI/UIObject/SelectionWidget.h"
#include "PlayerCountSelectionWidget.generated.h"

UCLASS()
class SPARTAFIGHTERS_API UPlayerCountSelectionWidget : public USelectionWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;
};
