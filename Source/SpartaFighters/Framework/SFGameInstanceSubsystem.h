#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "DataTypes/GameModeType.h"
#include "DataTable/MapInfoRow.h"
#include "Items/SFItemBase.h"
#include "Items/EquipItems/SFEquipableBase.h"
#include "Inventory/SFInventoryComponent.h"
#include "SFGameInstanceSubsystem.generated.h"

UENUM(BlueprintType)
enum class EGameState : uint8
{
	LoginMenu,
	LobbyMenu,
	RoomMenu,
	InGame,
	GameOver,
	Loading
};

class ASFGameModeBase;
class UDataTable;
class UUIManager;
class UUIManagerSettings;

UCLASS()
class SPARTAFIGHTERS_API USFGameInstanceSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
private:
	EGameState CurrentGameState;
	EGameModeType CurrentGameMode;

	UPROPERTY()
	ASFGameModeBase* CurrentGameModeInstance;

	UPROPERTY()
	UUIManager* UIManager;

	//Inventory
	TMap<FString, TArray<USFItemBase*>> PlayerInventories;
	TMap<FString, TArray<USFEquipableBase*>> PlayerEquipments;
	//Inventory when(no character)
	TMap<FString, TArray<TSubclassOf<class USFItemBase>>> PendingShopPurchases;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map Data")
	TObjectPtr<UDataTable> MapDataTable;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map Data")
	FMapInfoRow CurrentMapInfoRow;

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	EGameState GetCurrentGameState() const { return CurrentGameState; }
	void SetCurrentGameState(EGameState NewGameState);

	EGameModeType GetCurrentGameMode() const { return CurrentGameMode; }
	void SetCurrentGameMode(EGameModeType NewGameModeType);

	void ChangeLevelByMapName(const FString& MapName);
	void ChangeLevelByMapID(int32 MapID);

	UUIManager* GetUIManager() const { return UIManager; }
	
	UFUNCTION(BlueprintCallable)
	void ConnectToServerByAddress(const FString& ServerAddress);

	//Player Inventory update
	UFUNCTION(BlueprintCallable)
	void UpdatePlayerInventory(const FString& PlayerID, const TArray<USFItemBase*>& Inventory);

	//Player Equipment update
	UFUNCTION(BlueprintCallable)
	void UpdatePlayerEquipment(const FString& PlayerID, USFEquipableBase* Common, USFEquipableBase* Exclusive, USFEquipableBase* Cosmetic);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	TArray<USFItemBase*> GetPlayerInventory(const FString& PlayerID) const;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	const USFEquipableBase* GetPlayerEquippedItem(const FString& PlayerID, SFEquipSlot Slot) const;

	//(Inventory when no character)
	UFUNCTION(BlueprintCallable)
	void AddPendingShopPurchase(const FString& PlayerID, TSubclassOf<class USFItemBase> ItemClass);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	TArray<TSubclassOf<class USFItemBase>> GetPendingShopPurchases(const FString& PlayerID) const;

	UFUNCTION(BlueprintCallable)
	void ClearPendingShopPurchases(const FString& PlayerID);
};

