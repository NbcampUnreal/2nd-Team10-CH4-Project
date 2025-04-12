#include "Framework/SFPlayerState.h"
#include "Framework/SFGameInstanceSubsystem.h"
#include "UI/UIManager/UIManager.h"
#include "UI/UIElements/RoomWidget.h"
#include "Net/UnrealNetwork.h"

ASFPlayerState::ASFPlayerState()
{
	bReplicates = true;
}

void ASFPlayerState::OnRep_bIsReady()
{
	// ���� �ʿ� �� Ŭ���̾�Ʈ���� RoomWidget ������Ʈ ����
}

void ASFPlayerState::OnRep_bIsRoomOwner()
{
	// Host ������ ���� �� UI ó��
}

FString ASFPlayerState::GetUniqueID() const
{
	return CustomPlayerID;
}

void ASFPlayerState::AddDeathCount()
{
	if (HasAuthority())
	{
		++DeathCount;
		UE_LOG(LogTemp, Log, TEXT("%s died. DeathCount = %d"), *GetPlayerName(), DeathCount);
	}
}

void ASFPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASFPlayerState, bIsReady);
	DOREPLIFETIME(ASFPlayerState, bIsRoomOwner);
	DOREPLIFETIME(ASFPlayerState, bIsAI);
	DOREPLIFETIME(ASFPlayerState, SelectedCharacterRow);
}