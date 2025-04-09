#include "Framework/SFGameModeBase.h"
#include "Character/SFCharacter.h"
#include "Framework/SFLobbyPlayerController.h"
#include "Framework/SFGameStateBase.h"

ASFGameModeBase::ASFGameModeBase()
{
	PlayerControllerClass = ASFLobbyPlayerController::StaticClass();
	GameStateClass = ASFGameStateBase::StaticClass();
}

void ASFGameModeBase::UpdateRoomInfo()
{
	UE_LOG(LogTemp, Warning, TEXT("UpdateRoomInfo Called!!"));
	if (ASFGameStateBase* SFGameState = GetGameState<ASFGameStateBase>())
	{
		SFGameState->Multicast_UpdateRoomInfo();
	}
}