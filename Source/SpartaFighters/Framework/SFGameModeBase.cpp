#include "Framework/SFGameModeBase.h"
#include "Character/SFCharacter.h"
#include "Framework/SFPlayerController.h"
#include "Framework/SFGameStateBase.h"

ASFGameModeBase::ASFGameModeBase()
{
	DefaultPawnClass = ASFCharacter::StaticClass();
	PlayerControllerClass = ASFPlayerController::StaticClass();
	GameStateClass = ASFGameStateBase::StaticClass();
}
