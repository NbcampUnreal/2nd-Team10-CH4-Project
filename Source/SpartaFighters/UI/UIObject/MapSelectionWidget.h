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

private:
    UPROPERTY()
    URoomWidget* RoomWidget;
    UPROPERTY(EditDefaultsOnly, Category = "Map")
    UDataTable* MapDataTable;
    UPROPERTY(meta = (BindWidget))
    UImage* MapThumbnail;

    TArray<FMapInfoRow*> AvailableMaps;
    int32 CurrentIndex = 0;

    void UpdateSelectionUI();

public:
    void SetRoomSettings(const FRoomSettings& InRoomSettings);
    void UpdateAvailableMaps();
    FMapInfo GetCurrentSelectedMap() const;
    void SetGameMode(EGameModeType InGameMode);

private:
    EGameModeType CurrentGameMode;
};
