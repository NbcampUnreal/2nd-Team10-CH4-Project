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

    EGameMode CurrentGameMode = RoomWidget->GetCurrentRoomInfo().GameMode;

    TArray<FMapInfoRow*> FilteredMaps;
    static const FString ContextString(TEXT("FilteredMapsDataContext"));
    MapDataTable->GetAllRows<FMapInfoRow>(TEXT("ContextString"), FilteredMaps);

    AvailableMaps.Empty();
	Options.Empty();

    for (FMapInfoRow* MapRow : FilteredMaps)
    {
        if (MapRow && MapRow->MapInfo.GameMode == CurrentGameMode)
        {
            AvailableMaps.Add(MapRow);
            Options.Add(AvailableMaps[CurrentIndex]->MapInfo.MapName);
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