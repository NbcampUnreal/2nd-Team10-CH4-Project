#pragma once

#include "CoreMinimal.h"
#include "UI/Popup/SelectPopUpBase.h"
#include "DataTable/CharacterDataRow.h"
#include "SelectCharacterWidget.generated.h"

class UCharacterSlotWidget;
class UUniformGridPanel;
class UComboBoxString;
class UButton;
class UDataTable;

UCLASS()
class SPARTAFIGHTERS_API USelectCharacterWidget : public USelectPopUpBase
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

protected:
	UPROPERTY(meta = (BindWidget))
	UUniformGridPanel* CharacterGrid;
	UPROPERTY(meta = (BindWidget))
	UComboBoxString* CommonItemComboBox;
	UPROPERTY(meta = (BindWidget))
	UComboBoxString* ExclusiveItemComboBox;
	UPROPERTY(meta = (BindWidget))
	UComboBoxString* CosmeticItemComboBox;
	UPROPERTY(meta = (BindWidget))
	UButton* ConfirmButton;

	UPROPERTY(EditDefaultsOnly, Category = "Data")
	UDataTable* CharacterDataTable;
	UPROPERTY(EditDefaultsOnly, Category = "Widget")
	TSubclassOf<UCharacterSlotWidget> CharacterSlotWidgetClass;
	TArray<FCharacterDataRow*> AvailableCharacters;

	UPROPERTY()
	FName SelectedCharacterRow;

	UFUNCTION()
	void OnCharacterSlotClicked(FName RowName);
	//UFUNCTION()
	//void OnConfirmClicked();
	void UpdateAvailableCharacters();
	bool IsCharacterUnlocked(FName CharacterName) const;
	void PopulateCharacterGrid();
};
