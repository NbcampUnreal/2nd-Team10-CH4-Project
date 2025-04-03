#pragma once

#include "CoreMinimal.h"
#include "UI/UIObject/SelectionWidget.h"
#include "DataTable/MapInfoRow.h"
#include "MapSelectionWidget.generated.h"

class UTextBlock;
class UButton;
class UImage;
class URoomWidget;

UCLASS()
class SPARTAFIGHTERS_API UMapSelectionWidget : public USelectionWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
	UImage* MapThumbnail;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map Selection")
	UDataTable* MapDataTable;

	TArray<FMapInfoRow*> AvailableMaps;

public:
	void UpdateAvailableMaps();
	void SetRoomWidget(URoomWidget* InRoomWidget);
	FMapInfo GetCurrentSelectedMap() const;

private:
	TSoftObjectPtr<URoomWidget> RoomWidget;
	void UpdateSelectionUI();
};
