#include "Framework/SFGameModeBase.h"
#include "Character/SFCharacter.h"
#include "Framework/SFLobbyPlayerController.h"
#include "Framework/SFGameStateBase.h"

ASFGameModeBase::ASFGameModeBase()
{
	PlayerControllerClass = ASFLobbyPlayerController::StaticClass();
	GameStateClass = ASFGameStateBase::StaticClass();
}
