#include "MapSelectionWidget.h"

#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/Button.h"

#include "UI/UIElements/RoomWidget.h"
#include "Framework/SFRoomPlayerController.h"
#include "DataTable/MapInfoRow.h"
#include "Kismet/GameplayStatics.h"

void UMapSelectionWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (StartButton)
	{
		StartButton->OnClicked.AddUniqueDynamic(this, &UMapSelectionWidget::OnStartButtonClicked);
	}
	if (LeftArrowButton)
	{
		LeftArrowButton->OnClicked.AddUniqueDynamic(this, &UMapSelectionWidget::OnLeftArrowClicked);
	}
	if (RightArrowButton)
	{
		RightArrowButton->OnClicked.AddUniqueDynamic(this, &UMapSelectionWidget::OnRightArrowClicked);
	}
}

void UMapSelectionWidget::NativeDestruct()
{
	Super::NativeDestruct();

	if (StartButton)
	{
		StartButton->OnClicked.RemoveDynamic(this, &UMapSelectionWidget::OnStartButtonClicked);
	}
	if (LeftArrowButton)
	{
		LeftArrowButton->OnClicked.RemoveDynamic(this, &UMapSelectionWidget::OnLeftArrowClicked);
	}
	if (RightArrowButton)
	{
		RightArrowButton->OnClicked.RemoveDynamic(this, &UMapSelectionWidget::OnRightArrowClicked);
	}
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
		UTexture2D* ThumbnailTex = AvailableMaps[CurrentIndex]->MapInfo.MapThumbnail.LoadSynchronous();
		if (!ThumbnailTex)
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to load texture for map: %s"), *AvailableMaps[CurrentIndex]->MapInfo.MapName);
		}
		MapThumbnail->SetBrushFromTexture(ThumbnailTex);
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
	UpdateSelectionUI();
}

void UMapSelectionWidget::OnStartButtonClicked()
{
	UE_LOG(LogTemp, Warning, TEXT("OnStartButtonClicked"));
	APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
	if (ASFRoomPlayerController* SFPC = Cast<ASFRoomPlayerController>(PC))
	{
		const FString SelectedMapName = GetCurrentSelectedMap().MapName;
		SFPC->Server_RequestLevelChangeByMapName(SelectedMapName);
	}
}

void UMapSelectionWidget::OnLeftArrowClicked()
{
	if (Options.Num() == 0)
	{
		return;
	}

	CurrentIndex = (CurrentIndex - 1 + Options.Num()) % Options.Num();
	SelectionText->SetText(FText::FromString(Options[CurrentIndex]));

	UpdateSelectionUI();

}

void UMapSelectionWidget::OnRightArrowClicked()
{
	if (Options.Num() == 0)
	{
		return;
	}

	CurrentIndex = (CurrentIndex + 1) % Options.Num();
	SelectionText->SetText(FText::FromString(Options[CurrentIndex]));

	UpdateSelectionUI();

}