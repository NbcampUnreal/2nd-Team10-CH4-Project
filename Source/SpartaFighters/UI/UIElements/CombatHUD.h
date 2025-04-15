#pragma once

#include "CoreMinimal.h"
#include "UI/BaseUserWidget.h"
#include "CombatHUD.generated.h"

class UUniformGridPanel;
class UCharacterStatusWidget;
class UHorizontalBox;

UCLASS()
class SPARTAFIGHTERS_API UCombatHUD : public UBaseUserWidget
{
	GENERATED_BODY()

public:
	void NativeConstruct() override;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UCharacterStatusWidget> CharacterStatusWidget;

	UPROPERTY(meta = (BindWidget))
	UHorizontalBox* StatusContainer;

	void CreateCharacterStatusWidget();

	UFUNCTION()
	void RebindCharacterStatus(APawn* NewPawn);

private:
	FTimerHandle InitCreateWidgetTimer;
};
