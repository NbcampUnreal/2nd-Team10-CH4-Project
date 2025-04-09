#include "Framework/SFGameStateBase.h"
#include "Framework/SFGameInstanceSubsystem.h"
#include "Framework/SFPlayerState.h"
#include "Framework/SFLobbyPlayerController.h"
#include "UI/UIManager/UIManager.h"
#include "UI/UIElements/RoomWidget.h"
#include "Net/UnrealNetwork.h"

ASFGameStateBase::ASFGameStateBase()
{
	bReplicates = true;
}

void ASFGameStateBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASFGameStateBase, RoomOwner);
	DOREPLIFETIME(ASFGameStateBase, RoomList);
}

void ASFGameStateBase::SetRoomOwner(ASFPlayerState* NewOwner)
{
	if (HasAuthority())
	{
		RoomOwner = NewOwner;
	}
}

bool ASFGameStateBase::AreAllPlayersReady() const
{
	for (APlayerState* PlayerState : PlayerArray)
	{
		if (ASFPlayerState* SFPlayerState = Cast<ASFPlayerState>(PlayerState))
		{
			if (SFPlayerState != RoomOwner)
			{
				if (!SFPlayerState->PlayerInfo.bIsReady)
				{
					return false;
				}
			}
		}
	}
	return true;
}

void ASFGameStateBase::Server_ToggleReady_Implementation(ASFPlayerState* PlayerState)
{
	if (PlayerState)
	{
		PlayerState->PlayerInfo.bIsReady = !PlayerState->PlayerInfo.bIsReady;
	}
}

void ASFGameStateBase::Server_StartGame_Implementation(ASFPlayerState* RequestingPlayer)
{
	if (RequestingPlayer == RoomOwner)
	{
		if (AreAllPlayersReady())
		{
			UE_LOG(LogTemp, Log, TEXT("All players ready! Starting the game..."));
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Not all players are ready."));
		}
	}
}

void ASFGameStateBase::Server_CreateRoom_Implementation(const FRoomSettings& RoomSettings, APlayerController* OwnerPlayer)
{
	UE_LOG(LogTemp, Warning, TEXT("=== Server_CreateRoom_Implementation Called ==="));

	if (!OwnerPlayer)
	{
		UE_LOG(LogTemp, Error, TEXT("[Server_CreateRoom] OwnerPlayer is NULL!"));
		return;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[Server_CreateRoom] OwnerPlayer: %s"), *OwnerPlayer->GetName());
	}

	// 1. Create a new room
	FRoomInfo NewRoom;
	NewRoom.RoomName = RoomSettings.RoomName;
	NewRoom.MaxPlayers = RoomSettings.PlayerCount;
	NewRoom.CurrentPlayers = 1;
	NewRoom.GameMode = RoomSettings.GameMode;
	NewRoom.RoomID = FMath::Rand();
	NewRoom.bIsGameInProgress = false;

	FString OwnerPlayerID = TEXT("Unknown");

	// 2. Check if OwnerPlayer is valid
	ASFPlayerState* SFPlayerState = OwnerPlayer->GetPlayerState<ASFPlayerState>();
	if (SFPlayerState)
	{
		OwnerPlayerID = SFPlayerState->GetPlayerInfoID();
		UE_LOG(LogTemp, Warning, TEXT("[Server_CreateRoom] Found PlayerState. OwnerPlayerID: %s"), *OwnerPlayerID);

		FPlayerInfo NewPlayer = SFPlayerState->PlayerInfo;
		NewRoom.PlayerList.AddPlayer(NewPlayer);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("[Server_CreateRoom] OwnerPlayer does not have ASFPlayerState"));
	}
	NewRoom.OwnerPlayerID = OwnerPlayerID;

	// 3. Add to RoomList
	RoomList.Add(NewRoom);
	UE_LOG(LogTemp, Warning, TEXT("[Server_CreateRoom] Room '%s' created and added to RoomList."), *NewRoom.RoomName);


	// 4. Debug: Print current RoomList
	UE_LOG(LogTemp, Warning, TEXT("[Server_CreateRoom] Current RoomList.Num: %d"), RoomList.Num());
	for (const FRoomInfo& Room : RoomList)
	{
		UE_LOG(LogTemp, Warning, TEXT("    RoomName: %s, OwnerID: %s, MaxPlayers: %d, CurrentPlayers: %d"),
			*Room.RoomName, *Room.OwnerPlayerID, Room.MaxPlayers, Room.CurrentPlayers);
	}

	// 5. Send updated RoomList to all clients
	UE_LOG(LogTemp, Warning, TEXT("PlayerArray.Num: %d"), PlayerArray.Num());
	for (APlayerState* PlayerState : PlayerArray)
	{
		if (!PlayerState)
		{
			UE_LOG(LogTemp, Error, TEXT("PlayerState is null in PlayerArray"));
			continue;
		}

		AController* OwnerController = Cast<AController>(PlayerState->GetOwner());
		if (!OwnerController)
		{
			UE_LOG(LogTemp, Error, TEXT("PlayerState Owner is not a Controller"));
			continue;
		}

		ASFLobbyPlayerController* LobbyPC = Cast<ASFLobbyPlayerController>(OwnerController);
		if (LobbyPC)
		{
			UE_LOG(LogTemp, Warning, TEXT("Sending RoomList to PlayerController: %s"), *LobbyPC->GetName());
			LobbyPC->Client_UpdateRoomList(RoomList);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Owner Controller is not a ASFLobbyPlayerController: %s"), *OwnerController->GetName());
		}
	}

	// 6. Let the room creator enter the room
	if (ASFLobbyPlayerController* LobbyOwnerPC = Cast<ASFLobbyPlayerController>(OwnerPlayer))
	{
		UE_LOG(LogTemp, Warning, TEXT("Sending EnterCreatedRoom to LobbyOwnerPC: %s"), *LobbyOwnerPC->GetName());
		LobbyOwnerPC->Client_EnterRoom(NewRoom);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("OwnerPlayer is not a ASFLobbyPlayerController"));
	}
	UE_LOG(LogTemp, Warning, TEXT("=== Server_CreateRoom_Implementation End ==="));
}

void ASFGameStateBase::CheckAndUpdateRoomOwner()
{
	if (HasAuthority())
	{
		if (RoomOwner == nullptr || !PlayerArray.Contains(RoomOwner))
		{
			for (APlayerState* PS : PlayerArray)
			{
				if (ASFPlayerState* NewOwner = Cast<ASFPlayerState>(PS))
				{
					SetRoomOwner(NewOwner);
					UE_LOG(LogTemp, Log, TEXT("New RoomOwner set: %s"), *NewOwner->GetPlayerName());
					break;
				}
			}
		}
	}
}

void ASFGameStateBase::Multicast_UpdateRoomInfo_Implementation()
{
	//UE_LOG(LogTemp, Error, TEXT("Multicast_UpdateRoomInfo Called!!"));
	//for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	//{
	//	if (ASFLobbyPlayerController* LobbyPC = Cast<ASFLobbyPlayerController>(It->Get()))
	//	{
	//		ASFPlayerState* SFPlayerState = LobbyPC->GetPlayerState<ASFPlayerState>();
	//		UE_LOG(LogTemp, Error, TEXT("%s : CurrentRoomID : %d"), *this->GetActorNameOrLabel(), SFPlayerState->PlayerInfo.CurrentRoomID);
	//		if (SFPlayerState->PlayerInfo.CurrentRoomID != -1) // if Players are not in lobby
	//		{
	//			if (USFGameInstanceSubsystem* Subsystem = GetGameInstance()->GetSubsystem<USFGameInstanceSubsystem>())
	//			{
	//				if (UUIManager* UIManager = Subsystem->GetUIManager())
	//				{
	//					URoomWidget* RoomWidgetInstance = Cast<URoomWidget>(UIManager->RoomWidgetClass);
	//					RoomWidgetInstance->UpdatePlayerList();
	//				}
	//			}
	//		}
	//	}
	//}
}

FRoomInfo* ASFGameStateBase::FindRoomByID(int32 InRoomID)
{
	for (FRoomInfo& Room : RoomList)
	{
		if (Room.RoomID == InRoomID)
		{
			return &Room;
		}
	}
	return nullptr;
}

void ASFGameStateBase::OnRep_RoomList()
{
	UE_LOG(LogTemp, Log, TEXT("OnRep_RoomList Called : %s"), *this->GetActorNameOrLabel());
	if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
	{
		if (ASFLobbyPlayerController* LobbyPC = Cast<ASFLobbyPlayerController>(PC))
		{
			LobbyPC->Client_UpdateRoomList(RoomList);
		}
	}
}

void ASFGameStateBase::OnRep_RoomOwner()
{
	UE_LOG(LogTemp, Warning, TEXT("OnRep_RoomOwner Called!"));

	if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
	{
		if (ASFLobbyPlayerController* LobbyPC = Cast<ASFLobbyPlayerController>(PC))
		{
			FString NewOwnerPlayerID;

			if (RoomOwner)
			{
				NewOwnerPlayerID = RoomOwner->PlayerInfo.PlayerID;
			}

			LobbyPC->UpdatePlayerSlots(NewOwnerPlayerID);
		}
	}
}

