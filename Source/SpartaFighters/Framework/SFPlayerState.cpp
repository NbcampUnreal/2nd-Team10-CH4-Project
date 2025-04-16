#include "Framework/SFPlayerState.h"
#include "Framework/SFGameInstanceSubsystem.h"
#include "UI/UIManager/UIManager.h"
#include "UI/UIElements/RoomWidget.h"
#include "Net/UnrealNetwork.h"

ASFPlayerState::ASFPlayerState()
{
	bReplicates = true;
	CurrentGold = 500.0f;
}

void ASFPlayerState::OnRep_bIsReady()
{
	// 갱신 필요 시 클라이언트에서 RoomWidget 업데이트 가능
}

void ASFPlayerState::OnRep_bIsRoomOwner()
{
	// Host 아이콘 갱신 등 UI 처리
}

void ASFPlayerState::OnRep_SelectedCharacterRow()
{
	UE_LOG(LogTemp, Warning, TEXT("OnRep_SelectedCharacterRow called after travel: %s"), *SelectedCharacterRow.ToString());
}

void ASFPlayerState::CopyProperties(APlayerState* PlayerState)
{
	Super::CopyProperties(PlayerState);
	UE_LOG(LogTemp, Warning, TEXT("CopyProperties Called SelectedCharacterRow : %s"), *SelectedCharacterRow.ToString());
	if (ASFPlayerState* NewState = Cast<ASFPlayerState>(PlayerState))
	{
		NewState->SelectedCharacterRow = this->SelectedCharacterRow;
		NewState->CustomPlayerID = this->CustomPlayerID;
	UE_LOG(LogTemp, Warning, TEXT("CopyProperties SelectedCharacterRow : %s"), *SelectedCharacterRow.ToString());
	}
}

FString ASFPlayerState::GetUniqueID() const
{
	return CustomPlayerID;
}

FString ASFPlayerState::BP_GetUnique_ID() const
{
	return CustomPlayerID;
}

void ASFPlayerState::BP_SetSelectedCharacterRow(FName NewRow)
{
	SelectedCharacterRow = NewRow;
	UE_LOG(LogTemp, Warning, TEXT("Server: SetSelectedCharacterRow to %s"), *NewRow.ToString());
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

bool ASFPlayerState::AddGold(float Amount)
{
	if (Amount > 0)
	{
		CurrentGold += Amount;
		return true;
	}
	return false;
}

bool ASFPlayerState::RemoveGold(float Amount)
{
	if (Amount > 0 && CurrentGold >= Amount)
	{
		CurrentGold -= Amount;
		return true;
	}
	return false;
}

//Set Inventory
void ASFPlayerState::SetCharacterInventory(const TArray<USFItemBase*>& NewInventory)
{
	CharacterInventory = NewInventory;
}

//Set Equipment
void ASFPlayerState::SetCharacterEquipment(USFEquipableBase* Common, USFEquipableBase* Exclusive, USFEquipableBase* Cosmetic)
{
	CharacterEquippedCommon = Common;
	CharacterEquippedExclusive = Exclusive;
	CharacterEquippedCosmetic = Cosmetic;
}

void ASFPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASFPlayerState, bIsReady);
	DOREPLIFETIME(ASFPlayerState, bIsRoomOwner);
	DOREPLIFETIME(ASFPlayerState, bIsAI);
	DOREPLIFETIME(ASFPlayerState, SelectedCharacterRow);
	DOREPLIFETIME(ASFPlayerState, CustomPlayerID);
	DOREPLIFETIME(ASFPlayerState, CurrentGold);
	DOREPLIFETIME(ASFPlayerState, CharacterInventory);
	DOREPLIFETIME(ASFPlayerState, CharacterEquippedCommon);
	DOREPLIFETIME(ASFPlayerState, CharacterEquippedExclusive);
	DOREPLIFETIME(ASFPlayerState, CharacterEquippedCosmetic);
}

