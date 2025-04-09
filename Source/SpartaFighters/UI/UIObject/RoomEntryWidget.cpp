#include "UI/UIObject/RoomEntryWidget.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"

#include "Framework/SFLobbyPlayerController.h"

void URoomEntryWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (JoinRoomButton)
    {
        JoinRoomButton->OnClicked.AddDynamic(this, &URoomEntryWidget::OnJoinRoomClicked);
    }
}

void URoomEntryWidget::SetRoomInfo(const FRoomInfo& NewRoomInfo)
{
    RoomInfo = NewRoomInfo;
    if (RoomIDText)
    {
		FString RoomIDString = FString::Printf(TEXT("%d"), RoomInfo.RoomID);
        RoomIDText->SetText(FText::FromString(RoomIDString));
    }
    if (RoomNameText)
    {
        RoomNameText->SetText(FText::FromString(RoomInfo.RoomName));
    }
    if (GameModeText)
    {
        const UEnum* EnumPtr = StaticEnum<EGameModeType>();
        if (EnumPtr)
        {
            GameModeText->SetText(EnumPtr->GetDisplayNameTextByValue(static_cast<int64>(RoomInfo.GameMode)));
        }
    }
    if (PlayerCountText)
    {
        FString PlayerInfo = FString::Printf(TEXT("%d / %d"), RoomInfo.CurrentPlayers, RoomInfo.MaxPlayers);
        PlayerCountText->SetText(FText::FromString(PlayerInfo));
    }
    if (RoomStatusText)
    {
        RoomStatusText->SetText(RoomInfo.bIsGameInProgress ? FText::FromString("Playing") : FText::FromString("Waiting..."));
    }
}

void URoomEntryWidget::OnJoinRoomClicked()
{
    UE_LOG(LogTemp, Log, TEXT("OnJoinRoomClicked : %d"), RoomInfo.RoomID);

    if (APlayerController* PC = GetOwningPlayer())
    {
        if (ASFLobbyPlayerController* LobbyPC = Cast<ASFLobbyPlayerController>(PC))
        {
            LobbyPC->Server_JoinRoom(RoomInfo.RoomID);
        }
    }
}