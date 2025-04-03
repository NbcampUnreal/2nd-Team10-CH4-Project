#pragma once

#include "CoreMinimal.h"
#include "UI/PopUp/SelectPopUpBase.h"
#include "CreateRoomWidget.generated.h"

USTRUCT(BlueprintType)
struct FRoomSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	FString RoomName;
	UPROPERTY(EditAnywhere)
	FString GameMode;
	UPROPERTY(EditAnywhere)
	int32 PlayerCount;
	UPROPERTY(EditAnywhere)
	bool bItemEnabled;
};

class UTextBlock;
class UGameModeSelectionWidget;
class UPlayerCountSelectionWidget;
class UItemActivationSelectionWidget;
class URoomWidget;

UCLASS()
class SPARTAFIGHTERS_API UCreateRoomWidget : public USelectPopUpBase
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Room")
	TSoftClassPtr<URoomWidget> RoomWidgetClass;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* RoomNameText;
	UPROPERTY(meta = (BindWidget))
	TSoftObjectPtr<UGameModeSelectionWidget> GameModeSelectionWidget;
	UPROPERTY(meta = (BindWidget))
	TSoftObjectPtr<UPlayerCountSelectionWidget> PlayerCountSelectionWidget;
	UPROPERTY(meta = (BindWidget))
	TSoftObjectPtr<UItemActivationSelectionWidget> ItemActivationSelectionWidget;
	
	UPROPERTY(meta = (BindWidget))
	UButton* CreateRoomButton;

	UFUNCTION(BlueprintCallable)
	void CreateAndOpenRoomWidget();
	UFUNCTION()
	FString GenerateRandomRoomName();

public:
	UFUNCTION()
	FRoomSettings GetRoomSettings() const;
	UFUNCTION()
	void OnGameModeChanged(int32 SelectedIndex);
	UFUNCTION()
	void ResetRoomNameText();
};
