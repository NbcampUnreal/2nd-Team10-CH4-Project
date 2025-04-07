#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BaseUserWidget.generated.h"

class UUIManager;

UCLASS()
class SPARTAFIGHTERS_API UBaseUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UUIManager* ResolveUIManager() const;
};