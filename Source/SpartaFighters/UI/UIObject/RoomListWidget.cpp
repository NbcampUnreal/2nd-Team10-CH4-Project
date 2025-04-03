#include "UI/UIObject/RoomListWidget.h"
#include "UI/UIObject/RoomEntryWidget.h"
#include "Components/ScrollBox.h"
#include "Components/TextBlock.h"

void URoomListWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (!RoomScrollBox)
	{
		UE_LOG(LogTemp, Error, TEXT("RoomScrollBox is not assigned in the blueprint!"));
	}
}

void URoomListWidget::UpdateRoomList(const TArray<FRoomInfo>& NewRoomList)
{
	if (!RoomScrollBox || !RoomEntryWidgetClass)
	{
		return;
	}

	RoomScrollBox->ClearChildren();
	RoomList = NewRoomList;

	for (const FRoomInfo& Room : RoomList)
	{
		URoomEntryWidget* RoomEntry = CreateWidget<URoomEntryWidget>(GetWorld(), RoomEntryWidgetClass);
		if (RoomEntry)
		{
			RoomEntry->SetRoomInfo(Room);
			RoomScrollBox->AddChild(RoomEntry);
		}
	}
}

void URoomListWidget::OnRoomSelected(const FRoomInfo& SelectedRoom)
{
	UE_LOG(LogTemp, Log, TEXT("Selected Room: %s"), *SelectedRoom.RoomName);
}