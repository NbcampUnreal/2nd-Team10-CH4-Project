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

void ASFPlayerState::OnRep_SelectedCharacterRow()
{
	UE_LOG(LogTemp, Warning, TEXT("OnRep_SelectedCharacterRow called after travel: %s"), *SelectedCharacterRow.ToString());
}

void ASFPlayerState::CopyProperties(APlayerState* PlayerState)
{
	Super::CopyProperties(PlayerState);
	UE_LOG(LogTemp, Warning, TEXT("SelectedCharacterRow : %s"), *SelectedCharacterRow.ToString());
	if (ASFPlayerState* OldState = Cast<ASFPlayerState>(PlayerState))
	{
		SelectedCharacterRow = OldState->SelectedCharacterRow;
	}
}


FString ASFPlayerState::GetUniqueID() const
{
	return CustomPlayerID;
}

void ASFPlayerState::SetSelectedCharacterRow(FName NewRow)
{
	SelectedCharacterRow = NewRow;
	UE_LOG(LogTemp, Warning, TEXT("Server: SetSelectedCharacterRow to %s"), *NewRow.ToString());
}

FName ASFPlayerState::GetSelectedCharacterRow() const
{
	return SelectedCharacterRow;
}

void ASFPlayerState::AddDeathCount()
{
	if (HasAuthority())
	{
		++DeathCount;
		UE_LOG(LogTemp, Log, TEXT("%s died. DeathCount = %d"), *GetPlayerName(), DeathCount);
	}
}

FString ASFPlayerState::PrintSelectedCharacterRow()
{
	FString SelectedChar = GetSelectedCharacterRow().ToString();
	return SelectedChar;
}

void ASFPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASFPlayerState, bIsReady);
	DOREPLIFETIME(ASFPlayerState, bIsRoomOwner);
	DOREPLIFETIME(ASFPlayerState, bIsAI);
	DOREPLIFETIME(ASFPlayerState, SelectedCharacterRow);
}