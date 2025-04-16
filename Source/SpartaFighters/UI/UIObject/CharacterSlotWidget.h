#pragma once

#include "CoreMinimal.h"
#include "UI/BaseUserWidget.h"
#include "DataTable/CharacterDataRow.h"
#include "CharacterSlotWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCharacterSlotClicked, FName, RowName);

UCLASS()
class SPARTAFIGHTERS_API UCharacterSlotWidget : public UBaseUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	void Setup(const FCharacterDataRow& InData);

	UPROPERTY(BlueprintAssignable)
	FOnCharacterSlotClicked OnClicked;

	UPROPERTY(BlueprintReadOnly, Category = "Character")
	FName RowName;

protected:
	UPROPERTY(meta = (BindWidget))
	class UButton* SelectButton;

	UPROPERTY(meta = (BindWidget))
	class UImage* CharacterThumbnail;

	UFUNCTION()
	void HandleClick();
};
