#include "UI/UIObject/MapSelectionWidget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "UI/RoomWidget.h"
#include "DataTable/MapInfoRow.h"

void UMapSelectionWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UMapSelectionWidget::SetRoomSettings(const FRoomSettings& InRoomSettings)
{
	CurrentGameMode = InRoomSettings.GameMode;
	UpdateAvailableMaps();
}

void UMapSelectionWidget::UpdateAvailableMaps()
{
	if (!MapDataTable)
	{
		return;
	}

	TArray<FMapInfoRow*> FilteredMaps;
	static const FString ContextString(TEXT("FilteredMapsDataContext"));
	MapDataTable->GetAllRows<FMapInfoRow>(ContextString, FilteredMaps);

	AvailableMaps.Empty();
	Options.Empty();

	for (FMapInfoRow* MapRow : FilteredMaps)
	{
		if (MapRow && MapRow->MapInfo.GameMode == CurrentGameMode) 
		{
			AvailableMaps.Add(MapRow);
			Options.Add(MapRow->MapInfo.MapName);
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
		if (SelectionText)
		{
			SelectionText->SetText(FText::FromString(AvailableMaps[CurrentIndex]->MapInfo.MapName));
		}
	}
}

FMapInfo UMapSelectionWidget::GetCurrentSelectedMap() const
{
	return AvailableMaps.IsValidIndex(CurrentIndex) ? AvailableMaps[CurrentIndex]->MapInfo : FMapInfo();
}

void UMapSelectionWidget::SetGameMode(EGameModeType InGameMode)
{
	CurrentGameMode = InGameMode;
	UpdateAvailableMaps();
}
