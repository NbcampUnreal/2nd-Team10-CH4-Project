#include "UI/UIObject/RoomEntryWidget.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"

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

    if (RoomNameText)
    {
        RoomNameText->SetText(FText::FromString(RoomInfo.RoomName));
    }
    if (GameModeText)
    {
        GameModeText->SetText(FText::FromString(RoomInfo.GameMode));
    }
    if (MapNameText)
    {
        MapNameText->SetText(FText::FromString(RoomInfo.MapName));
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
    UE_LOG(LogTemp, Log, TEXT("�� ���� �õ�: %d"), RoomInfo.RoomID);

    // �� ���� ó�� (��Ʈ��ũ ��û �Ǵ� ���� ��� ��ȯ)
}