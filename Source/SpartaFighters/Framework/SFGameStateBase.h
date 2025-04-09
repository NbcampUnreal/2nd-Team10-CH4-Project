#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "Framework/SFPlayerState.h"
#include "DataTypes/RoomInfo.h"
#include "DataTypes/PlayerInfo.h"
#include "UI/PopUp/CreateRoomWidget.h"
#include "SFGameStateBase.generated.h"

UCLASS()
class SPARTAFIGHTERS_API ASFGameStateBase : public AGameStateBase
{
	GENERATED_BODY()
	
public:
    ASFGameStateBase();

    UPROPERTY(ReplicatedUsing = OnRep_RoomOwner, BlueprintReadOnly, Category = "Game")
    ASFPlayerState* RoomOwner;

    UFUNCTION(BlueprintCallable, Category = "Game")
    bool AreAllPlayersReady() const;

    // Run on Server
    void SetRoomOwner(ASFPlayerState* NewOwner);

    void CheckAndUpdateRoomOwner();

    UFUNCTION(Server, Reliable)
    void Server_ToggleReady(ASFPlayerState* PlayerState);

    UFUNCTION(Server, Reliable)
    void Server_StartGame(ASFPlayerState* RequestingPlayer);

    UPROPERTY(ReplicatedUsing = OnRep_RoomList)
    TArray<FRoomInfo> RoomList;

    FRoomInfo* FindRoomByID(int32 InRoomID);
    
    UFUNCTION(Server, Reliable)
    void Server_CreateRoom(const FRoomSettings& RoomSettings, APlayerController* OwnerPlayer);

    UFUNCTION()
    void OnRep_RoomList();

    const TArray<FPlayerInfo>& GetPlayerList() const { return PlayerList.Items; }

    UFUNCTION(NetMulticast, Reliable)
    void Multicast_UpdateRoomInfo();

protected:
    UPROPERTY(Replicated)
    FPlayerInfoArray PlayerList;
    
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

    UFUNCTION()
    void OnRep_RoomOwner();
};
