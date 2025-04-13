#include "Framework/SFCooperativeGameMode.h"
#include "Framework/SFGameStateBase.h"
#include "Framework/SFPlayerState.h"

ASFCooperativeGameMode::ASFCooperativeGameMode()
{
	UE_LOG(LogTemp, Warning, TEXT("Cooperative Game Mode Initialized"));
	GameStateClass = ASFGameStateBase::StaticClass();
	PlayerStateClass = ASFPlayerState::StaticClass();

	static ConstructorHelpers::FObjectFinder<UDataTable> DT_CharacterData(TEXT("/Game/SpartaFighters/DataAsset/DataTable/DT_CharacterData.DT_CharacterData"));
	if (DT_CharacterData.Succeeded())
	{
		CharacterDataTable = DT_CharacterData.Object;
	}
}