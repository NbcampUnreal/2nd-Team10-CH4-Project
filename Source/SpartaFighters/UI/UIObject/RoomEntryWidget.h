#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DataTypes/RoomInfo.h"
#include "RoomEntryWidget.generated.h"

class UTextBlock;
class UButton;

UCLASS()
class SPARTAFIGHTERS_API URoomEntryWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void SetRoomInfo(const FRoomInfo& NewRoomInfo);

protected:
    virtual void NativeConstruct() override;

private:
    FRoomInfo RoomInfo;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* RoomIDText;
    UPROPERTY(meta = (BindWidget))
    UTextBlock* RoomNameText;
    UPROPERTY(meta = (BindWidget))
    UTextBlock* GameModeText;
    UPROPERTY(meta = (BindWidget))
    UTextBlock* PlayerCountText;
    UPROPERTY(meta = (BindWidget))
    UTextBlock* RoomStatusText;
    UPROPERTY(meta = (BindWidget))
    UButton* JoinRoomButton;

    UFUNCTION()
    void OnJoinRoomClicked();
};
