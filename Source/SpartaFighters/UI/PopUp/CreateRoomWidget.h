#pragma once

#include "CoreMinimal.h"
#include "UI/PopUp/SelectPopUpBase.h"
#include "CreateRoomWidget.generated.h"

class UComboBoxString;
class UCheckBox;
class UTextBlock;

UCLASS()
class SPARTAFIGHTERS_API UCreateRoomWidget : public USelectPopUpBase
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

	FString GenerateRandomRoomName();

public:
	UPROPERTY(meta = (BindWidget))
	UComboBoxString* ModeComboBox;
	UPROPERTY(meta = (BindWidget))
	UComboBoxString* PlayerCountComboBox;
	UPROPERTY(meta = (BindWidget))
	UCheckBox* ItemCheckBox;
	UPROPERTY(meta = (BindWidget))
	UButton* CreateRoomButton;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* RoomNameText;

	UFUNCTION(BlueprintCallable)
	void OnCreateRoomButtonClicked();
	UFUNCTION()
	void ResetCreateRoomWidget();

};
