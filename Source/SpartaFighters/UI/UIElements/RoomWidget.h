#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DataTypes/RoomInfo.h"
#include "RoomWidget.generated.h"

class UTextBlock;
class URoomChatWidget;
class UButton;
class UPlayerSimpleInfoWidget;
class UMapSelectionWidget;

UCLASS()
class SPARTAFIGHTERS_API URoomWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void SetupRoom(const FRoomInfo& RoomInfo);

protected:
	virtual void NativeConstruct() override;

private:
	UPROPERTY(meta = (BindWidget))
	TSoftObjectPtr<UPlayerSimpleInfoWidget> PlayerSimpleInfoWidgetClass;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* RoomNameText;
	UPROPERTY(meta = (BindWidget))
	TSoftObjectPtr<URoomChatWidget> RoomChatWidgetClass;
	UPROPERTY(meta = (BindWidget))
	UButton* LobbyButton;

	UFUNCTION()
	void OnLobbyButtonClicked();

	FRoomInfo CurrentRoomInfo;
	void UpdatePlayerList();

public:
	UPROPERTY(meta = (BindWidget))
	TSoftObjectPtr<UMapSelectionWidget> MapSelectionWidgetClass;

	FRoomInfo GetCurrentRoomInfo() const { return CurrentRoomInfo; }
};
