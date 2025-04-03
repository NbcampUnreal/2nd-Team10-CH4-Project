#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DataTypes/RoomInfo.h"
#include "RoomListWidget.generated.h"

class UScrollBox;
class URoomEntryWidget;

UCLASS()
class SPARTAFIGHTERS_API URoomListWidget : public UUserWidget
{
	GENERATED_BODY()
	

public:
	void UpdateRoomList(const TArray<FRoomInfo>& NewRoomList);

protected:
	virtual void NativeConstruct() override;

private:
	UPROPERTY()
	TArray<FRoomInfo> RoomList;
	UPROPERTY(meta = (BindWidget))
	UScrollBox* RoomScrollBox;
	UPROPERTY(EditAnywhere, Category = "Room List")
	TSubclassOf<URoomEntryWidget> RoomEntryWidgetClass;
	
	void OnRoomSelected(const FRoomInfo& SelectedRoom);
};
