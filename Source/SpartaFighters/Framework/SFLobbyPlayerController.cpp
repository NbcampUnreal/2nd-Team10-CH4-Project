#include "Framework/SFLobbyPlayerController.h"
#include "Framework/SFGameInstanceSubsystem.h"
#include "Framework/SFGameModeBase.h"
#include "Framework/SFGameStateBase.h"
#include "Framework/SFPlayerState.h"
#include "Framework/SFCheatManager.h"

#include "UI/UIManager/UIManager.h"
#include "UI/UIObject/RoomListWidget.h"
#include "UI/UIObject/PlayerSlotWidget.h"
#include "Blueprint/WidgetBlueprintLibrary.h"

#include "Net/UnrealNetwork.h"

ASFLobbyPlayerController::ASFLobbyPlayerController()
{
	CheatClass = USFCheatManager::StaticClass();

	bReplicates = true;
	bIsReady = false;
}

void ASFLobbyPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (IsLocalController())
	{
		if (USFGameInstanceSubsystem* Subsystem = GetGameInstance()->GetSubsystem<USFGameInstanceSubsystem>())
		{
			if (UUIManager* UIManager = Subsystem->GetUIManager())
			{
				UIManager->ShowLobbyMenu();
			}
		}
	}
}

void ASFLobbyPlayerController::Server_ToggleReady_Implementation()
{
	bIsReady = !bIsReady;

	ASFPlayerState* SFPlayerState = GetPlayerState<ASFPlayerState>();
	if (SFPlayerState)
	{
		SFPlayerState->PlayerInfo.bIsReady = bIsReady;
	}
}

void ASFLobbyPlayerController::Client_UpdateReadyState_Implementation(bool bInIsReady)
{
	bIsReady = bInIsReady;
}

void ASFLobbyPlayerController::Server_StartGame_Implementation()
{
}

void ASFLobbyPlayerController::Server_RequestCreateRoom_Implementation(const FRoomSettings& RoomSettings)
{
	// [DEBUG] Server_RequestCreateRoom_Implementation
	UE_LOG(LogTemp, Warning, TEXT("Server_ReqyestCreateRoom Called!"));
	if (ASFGameStateBase* SFGameState = GetWorld()->GetGameState<ASFGameStateBase>())
	{
		SFGameState->Server_CreateRoom(RoomSettings, this);
		SFGameState->Multicast_UpdateRoomInfo();
	}
}

void ASFLobbyPlayerController::Client_UpdateRoomList_Implementation(const TArray<FRoomInfo>& UpdatedRoomList)
{
	//// [DEBUG] Client_UpdateRoomList_Implementation
	//UE_LOG(LogTemp, Warning, TEXT("========== Client_UpdateRoomList Called! =========="));
	//// [DEBUG] PlayerController Name
	//UE_LOG(LogTemp, Warning, TEXT("PlayerController Name: %s"), *GetName());
	//// [DEBUG] NetMode
	//switch (GetNetMode())
	//{
	//case NM_Standalone:
	//	UE_LOG(LogTemp, Warning, TEXT("NetMode: NM_Standalone"));
	//	break;
	//case NM_Client:
	//	UE_LOG(LogTemp, Warning, TEXT("NetMode: NM_Client (This is a true client)"));
	//	break;
	//case NM_ListenServer:
	//	UE_LOG(LogTemp, Warning, TEXT("NetMode: NM_ListenServer"));
	//	break;
	//case NM_DedicatedServer:
	//	UE_LOG(LogTemp, Warning, TEXT("NetMode: NM_DedicatedServer"));
	//	break;
	//default:
	//	UE_LOG(LogTemp, Warning, TEXT("NetMode: Unknown"));
	//	break;
	//}
	//// [DEBUG] UpdatedRoomList
	//UE_LOG(LogTemp, Warning, TEXT("UpdatedRoomList.Num() = %d"), UpdatedRoomList.Num());
	//for (const FRoomInfo& RoomInfo : UpdatedRoomList)
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("RoomName: %s, OwnerID: %s, MaxPlayers: %d, CurrentPlayers: %d"),
	//		*RoomInfo.RoomName,
	//		*RoomInfo.OwnerPlayerID,
	//		RoomInfo.MaxPlayers,
	//		RoomInfo.CurrentPlayers);
	//}

	// Find and Update RoomListWidget
	TArray<UUserWidget*> FoundWidgets;
	UWidgetBlueprintLibrary::GetAllWidgetsOfClass(GetWorld(), FoundWidgets, URoomListWidget::StaticClass(), false);

	bool bUpdatedWidget = false;
	for (UUserWidget* Widget : FoundWidgets)
	{
		if (URoomListWidget* RoomListWidget = Cast<URoomListWidget>(Widget))
		{
			RoomListWidget->UpdateRoomList(UpdatedRoomList);
			bUpdatedWidget = true;
			break;
		}
	}

	//// [DEBUG] Widget Update Result
	//if (bUpdatedWidget)
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("Successfully updated RoomListWidget."));
	//}
	//else
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("Failed to find and update RoomListWidget."));
	//}
	//UE_LOG(LogTemp, Warning, TEXT("============================================="));
}

void ASFLobbyPlayerController::Server_LeaveRoom_Implementation(int32 RoomID)
{
	ASFGameStateBase* SFGameState = GetWorld()->GetGameState<ASFGameStateBase>();
	if (!SFGameState)
	{
		UE_LOG(LogTemp, Error, TEXT("SFGameState is null"));
		return;
	}
	FRoomInfo* FoundRoom = SFGameState->FindRoomByID(RoomID);
	if (!FoundRoom)
	{
		UE_LOG(LogTemp, Error, TEXT("Room with ID %d not found"), RoomID);
		return;
	}
	ASFPlayerState* SFPlayerState = GetPlayerState<ASFPlayerState>();
	if (!SFPlayerState)
	{
		UE_LOG(LogTemp, Error, TEXT("SFPlayerState is null"));
		return;
	}

	FString LeavingPlayerID = SFPlayerState->PlayerInfo.PlayerID;
	UE_LOG(LogTemp, Warning, TEXT("Server_LeaveRoom: %s Player try to get out Room %d."), *LeavingPlayerID, RoomID);
	FoundRoom->PlayerList.RemovePlayer(LeavingPlayerID);
	FoundRoom->CurrentPlayers = FMath::Max(FoundRoom->CurrentPlayers - 1, 0);
	UE_LOG(LogTemp, Log, TEXT("Server_LeaveRoom: Left PlayerCount = %d"), FoundRoom->CurrentPlayers);
	if (FoundRoom->OwnerPlayerID == LeavingPlayerID)
	{
		UE_LOG(LogTemp, Log, TEXT("Server_LeaveRoom: The outgoing player is the room leader."));

		if (FoundRoom->CurrentPlayers > 0)
		{
			UE_LOG(LogTemp, Log, TEXT("Server_LeaveRoom: There are players left. Assign a new room leader."));

			// Designate a NewRoomOwner from remaining players
			for (APlayerState* CurrentPlayerState : SFGameState->PlayerArray)
			{
				ASFPlayerState* NextRoomOwnerState = Cast<ASFPlayerState>(CurrentPlayerState);
				if (NextRoomOwnerState && NextRoomOwnerState->PlayerInfo.CurrentRoomID == RoomID && NextRoomOwnerState->PlayerInfo.PlayerID != LeavingPlayerID)
				{
					FoundRoom->OwnerPlayerID = NextRoomOwnerState->PlayerInfo.PlayerID;
					SFGameState->SetRoomOwner(NextRoomOwnerState);
					UE_LOG(LogTemp, Log, TEXT("New Owner for Room %d: %s"), RoomID, *FoundRoom->OwnerPlayerID);
					break;
				}
			}
		}
		else
		{
			// 인원이 없으면 방을 삭제
			UE_LOG(LogTemp, Log, TEXT("Server_LeaveRoom: No one is in the room. Delete the room."));

			int32 RemoveCount = SFGameState->RoomList.RemoveSingle(*FoundRoom);
			if (RemoveCount > 0)
			{
				UE_LOG(LogTemp, Log, TEXT("Server_LeaveRoom: Room %d Deleted!"), RoomID);
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Server_LeaveRoom: Room %d Delete Failed! Not found in RoomList."), RoomID);
			}
		}
	}

	// 3. 나의 PlayerState Room 정보 초기화
	SFPlayerState->PlayerInfo.CurrentRoomID = -1; // 방 없음 표시
	UE_LOG(LogTemp, Log, TEXT("Server_LeaveRoom: %s Completed initializing the player's CurrentRoomID."), *LeavingPlayerID);

	Server_RequestUpdateRoomInfo();
}

void ASFLobbyPlayerController::Server_JoinRoom_Implementation(int32 InRoomID)
{
	ASFGameStateBase* SFGameState = GetWorld()->GetGameState<ASFGameStateBase>();
	if (!SFGameState)
	{
		return;
	}
	ASFPlayerState* SFPlayerState = GetPlayerState<ASFPlayerState>();
	if (!SFPlayerState)
	{
		return;
	}

	FRoomInfo* Room = SFGameState->FindRoomByID(InRoomID);
	if (!Room)
	{
		UE_LOG(LogTemp, Error, TEXT("Room with ID %d not found"), InRoomID);
		return;
	}
	FPlayerInfo NewPlayer = SFPlayerState->PlayerInfo;
	Room->PlayerList.AddPlayer(NewPlayer);
	Room->CurrentPlayers++;
	SFPlayerState->PlayerInfo.CurrentRoomID = InRoomID;

	Server_RequestUpdateRoomInfo();

	Client_EnterRoom(*Room);
}

void ASFLobbyPlayerController::Server_RequestUpdateRoomInfo_Implementation()
{
	if (HasAuthority()) // 서버에서 직접 실행할 때
	{
		if (ASFGameModeBase* GameMode = GetWorld()->GetAuthGameMode<ASFGameModeBase>())
		{
			GameMode->UpdateRoomInfo(); // GameMode 함수 호출
		}
	}
}

void ASFLobbyPlayerController::Client_EnterRoom_Implementation(const FRoomInfo& RoomInfo)
{
	if (USFGameInstanceSubsystem* Subsystem = GetGameInstance()->GetSubsystem<USFGameInstanceSubsystem>())
	{
		if (UUIManager* UIManager = Subsystem->GetUIManager())
		{
			UIManager->OpenRoomWidget(RoomInfo);
		}
	}
}

void ASFLobbyPlayerController::UpdatePlayerSlots(const FString& NewOwnerPlayerID)
{
	UE_LOG(LogTemp, Warning, TEXT("UpdatePlayerSlots Called!"));
	// Find and Update PlayerSlotWidget
	TArray<UUserWidget*> FoundWidgets;
	UWidgetBlueprintLibrary::GetAllWidgetsOfClass(GetWorld(), FoundWidgets, UPlayerSlotWidget::StaticClass(), false);
	for (UUserWidget* Widget : FoundWidgets)
	{
		if (UPlayerSlotWidget* PlayerSlotWidget = Cast<UPlayerSlotWidget>(Widget))
		{
			PlayerSlotWidget->UpdateRoomOwner(NewOwnerPlayerID);
		}
	}
}


void ASFLobbyPlayerController::InitUI()
{
	if (USFGameInstanceSubsystem* Subsystem = GetGameInstance()->GetSubsystem<USFGameInstanceSubsystem>())
	{
		if (UUIManager* UIManager = Subsystem->GetUIManager())
		{
			// Pass the DataAsset as well.
			UIManager->Init(this);
		}
	}
}