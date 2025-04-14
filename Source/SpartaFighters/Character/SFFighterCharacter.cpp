#include "Character/SFFighterCharacter.h"
#include "Character/AttackSystem/PunchAttack.h"

void ASFFighterCharacter::BeginPlay()
{
	Super::BeginPlay();
	// Example of adding an attack handler
	UPunchAttack* PunchAttack = NewObject<UPunchAttack>(this);
	//AddAttackHandler(PunchAttack);
}