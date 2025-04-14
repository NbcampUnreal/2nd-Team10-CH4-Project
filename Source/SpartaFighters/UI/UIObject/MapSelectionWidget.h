#pragma once

#include "CoreMinimal.h"
#include "UI/UIObject/SelectionWidget.h"
#include "DataTable/MapInfoRow.h"
#include "UI/PopUp/CreateRoomWidget.h"
#include "MapSelectionWidget.generated.h"

class UTextBlock;
class UImage;
class URoomWidget;

UCLASS()
class SPARTAFIGHTERS_API UMapSelectionWidget : public USelectionWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

public:
	UPROPERTY(meta = (BindWidget))
	UButton* StartButton;
	UPROPERTY(meta = (BindWidget))
	UImage* MapThumbnail;

	UPROPERTY(EditDefaultsOnly, Category = "Map")
	UDataTable* MapDataTable;
	TArray<FMapInfoRow*> AvailableMaps;

	int32 CurrentIndex = 0;

	UFUNCTION()
	void OnLeftArrowClicked();
	UFUNCTION()
	void OnRightArrowClicked();

	void UpdateSelectionUI();
	void UpdateAvailableMaps();
	FMapInfo GetCurrentSelectedMap() const;
	void SetGameMode(EGameModeType InGameMode);

	UFUNCTION()
	void OnStartButtonClicked();

private:
	EGameModeType CurrentGameMode;
};
