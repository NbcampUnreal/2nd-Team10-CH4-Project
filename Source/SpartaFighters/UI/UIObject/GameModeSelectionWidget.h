#pragma once

#include "CoreMinimal.h"
#include "UI/UIObject/SelectionWidget.h"
#include "GameModeSelectionWidget.generated.h"

UCLASS()
class SPARTAFIGHTERS_API UGameModeSelectionWidget : public USelectionWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;
};
