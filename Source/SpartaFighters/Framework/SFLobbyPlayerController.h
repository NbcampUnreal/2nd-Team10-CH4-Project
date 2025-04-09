#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "DataTypes/PlayerInfo.h"
#include "SFLobbyPlayerController.generated.h"

UCLASS()
class SPARTAFIGHTERS_API ASFLobbyPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
    ASFLobbyPlayerController();

protected:
    virtual void BeginPlay() override;

public:
	// RPCs called when a player toggles their ready state
    UFUNCTION(Server, Unreliable)
    void Server_ToggleReady();
    UFUNCTION(Client, Unreliable)
    void Client_UpdateReadyState(bool bInIsReady);
    UFUNCTION(Server, Unreliable)
    void Server_StartGame();
	// RPCs called when a room is created
    UFUNCTION(Server, Unreliable)
    void Server_RequestCreateRoom(const FRoomSettings& RoomSettings);
    UFUNCTION(Client, Reliable)
	void Client_UpdateRoomList(const TArray<FRoomInfo>& UpdatedRoomList);
	// RPCs called when a player leaves the room
    UFUNCTION(Server, Unreliable)
    void Server_LeaveRoom(int32 RoomID);
	// RPCs called when a player joins the room
    UFUNCTION(Server, Reliable)
    void Server_JoinRoom(int32 InRoomID);
    UFUNCTION(Server,Reliable)
    void Server_RequestUpdateRoomInfo();
    UFUNCTION(Client, Reliable)
    void Client_EnterRoom(const FRoomInfo& RoomInfo);

    UFUNCTION()
	void UpdatePlayerSlots(const FString& NewOwnerPlayerID);

    UPROPERTY(BlueprintReadOnly, Category = "Lobby")
    bool bIsReady;

private:
	void InitUI();

};
