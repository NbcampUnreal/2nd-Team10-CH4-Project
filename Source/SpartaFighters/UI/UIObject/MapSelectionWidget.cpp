#include "UI/UIObject/MapSelectionWidget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "UI/RoomWidget.h"

void UMapSelectionWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UMapSelectionWidget::SetRoomWidget(URoomWidget* InRoomWidget)
{
    RoomWidget = InRoomWidget;
    UpdateAvailableMaps();
}

void UMapSelectionWidget::UpdateAvailableMaps()
{
    if (!RoomWidget || !MapDataTable)  
    {
        return;
    }

    FString CurrentGameMode = RoomWidget->GetCurrentRoomInfo().GameMode;

    TArray<FMapInfoRow*> FilteredMaps;
    MapDataTable->GetAllRows<FMapInfoRow>(TEXT(""), FilteredMaps);

    AvailableMaps.Empty();

    for (FMapInfoRow* MapRow : FilteredMaps)
    {
        if (MapRow && MapRow->MapInfo.GameMode == CurrentGameMode)
        {
            Options.Add(AvailableMaps[CurrentIndex]->MapInfo.MapName);
            AvailableMaps.Add(MapRow);
        }
    }

    CurrentIndex = 0;
    UpdateSelectionUI();
}

void UMapSelectionWidget::UpdateSelectionUI()
{
    if (AvailableMaps.IsValidIndex(CurrentIndex))
    {
        if (MapThumbnail)
        {
            MapThumbnail->SetBrushFromTexture(AvailableMaps[CurrentIndex]->MapInfo.MapThumbnail.LoadSynchronous());
        }
    }
}

FMapInfo UMapSelectionWidget::GetCurrentSelectedMap() const
{
    return AvailableMaps.IsValidIndex(CurrentIndex) ? AvailableMaps[CurrentIndex]->MapInfo : FMapInfo();
}