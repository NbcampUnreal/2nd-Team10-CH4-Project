#include "SFGameInstanceSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "Framework/SFGameModeBase.h"
#include "Framework/SFSingleGameMode.h"
#include "Framework/SFBattleGameMode.h"
#include "Framework/SFCooperativeGameMode.h"
#include "Framework/SFGameInstance.h"
#include "DataTypes/GameModeType.h"
#include "UI/UIManager/UIManager.h"


void USFGameInstanceSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	if (!IsRunningDedicatedServer())
	{
		UIManager = NewObject<UUIManager>(GetGameInstance());
	}

	USFGameInstance* GameInstance = Cast<USFGameInstance>(GetGameInstance());
	if (GameInstance)
	{
		MapDataTable = GameInstance->MapDataTable;
		GameInstance->LoadMapData();
	}
}

void USFGameInstanceSubsystem::Deinitialize()
{
	Super::Deinitialize();
	CurrentGameModeInstance = nullptr;
	PlayerInventories.Empty();
	PlayerEquipments.Empty();
	PendingShopPurchases.Empty();
}

void USFGameInstanceSubsystem::SetCurrentGameState(EGameState NewGameState)
{
	if (CurrentGameState != NewGameState)
	{
		CurrentGameState = NewGameState;
		UE_LOG(LogTemp, Log, TEXT("Game State changed to: %s"), *UEnum::GetValueAsString(CurrentGameState));
	}
}

void USFGameInstanceSubsystem::SetCurrentGameMode(EGameModeType NewGameMode)
{
	CurrentGameMode = NewGameMode;
}

void USFGameInstanceSubsystem::ChangeLevelByMapName(const FString& MapName)
{
	const int32 MapID = GetTypeHash(MapName);
	ChangeLevelByMapID(MapID);
}

void USFGameInstanceSubsystem::ChangeLevelByMapID(int32 MapID)
{
	if (!MapDataTable)
	{
		UE_LOG(LogTemp, Warning, TEXT("MapDataTable is not assigned."));
		return;
	}

	static const FString ContextString(TEXT("MapInfo Context"));
	TArray<FMapInfoRow*> AllMaps;
	MapDataTable->GetAllRows<FMapInfoRow>(ContextString, AllMaps);

	for (FMapInfoRow* MapRow : AllMaps)
	{
		if (MapRow && MapRow->MapID == MapID)
		{
			FString LevelToLoad = MapRow->MapInfo.LevelPath;

			UWorld* World = GetWorld();
			if (!World)
			{
				UE_LOG(LogTemp, Error, TEXT("GetWorld() returned nullptr."));
				return;
			}

			APlayerController* PC = World->GetFirstPlayerController();
			if (!PC)
			{
				UE_LOG(LogTemp, Error, TEXT("No PlayerController found in the world."));
				return;
			}

			if (IsRunningDedicatedServer())
			{
				for (FConstPlayerControllerIterator It = World->GetPlayerControllerIterator(); It; ++It)
				{
					if (APlayerController* PlayerControllerInstance = It->Get())
					{
						if (ASFPlayerState* PS = PlayerControllerInstance->GetPlayerState<ASFPlayerState>())
						{
							UE_LOG(LogTemp, Warning, TEXT("[Before SeamlessTravel] PC: %s, Row: %s, PS : %s"),
								*PlayerControllerInstance->GetName(),
								*PS->GetSelectedCharacterRow().ToString(),
								*PS->GetActorNameOrLabel());
						}
						else
						{
							UE_LOG(LogTemp, Error, TEXT("PlayerController %s has no ASFPlayerState"), *PlayerControllerInstance->GetName());
						}
					}
				}

				/*UE_LOG(LogTemp, Log, TEXT("Changing Level to: %s (Server)"), *LevelToLoad);
				UGameplayStatics::OpenLevel(World, FName(*LevelToLoad), true);*/

				FName LevelName = FPackageName::GetShortFName(FName(*LevelToLoad));

				World->GetAuthGameMode()->bUseSeamlessTravel = true;
				World->ServerTravel(LevelName.ToString(), true);

				/*FString URL = FString::Printf(TEXT("%s?listen"), *LevelToLoad);
				GetWorld()->ServerTravel(URL);*/
			}
			else
			{
				UE_LOG(LogTemp, Log, TEXT("Changing Level to: %s (Client)"), *LevelToLoad);
				PC->ClientTravel(LevelToLoad, ETravelType::TRAVEL_Absolute);
			}
			return;
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("Map not found for ID: %d"), MapID);
}

void USFGameInstanceSubsystem::ConnectToServerByAddress(const FString& ServerAddress)
{
	UWorld* World = GetWorld();
	if (!World)
	{
		UE_LOG(LogTemp, Error, TEXT("ConnectToServerByAddress: World is null"));
		return;
	}

	APlayerController* PC = World->GetFirstPlayerController();
	if (!PC)
	{
		UE_LOG(LogTemp, Error, TEXT("ConnectToServerByAddress: PlayerController is null"));
		return;
	}

	UE_LOG(LogTemp, Log, TEXT("Client traveling to server: %s"), *ServerAddress);
	FString ServerURL = ServerAddress;
	///	When loading a save game, the URL should be different.
	FURL DedicatedServerURL(nullptr, *ServerURL, TRAVEL_Absolute);
	FString ErrorMessage;
	GEngine->Browse(GEngine->GetWorldContextFromWorldChecked(GetWorld()), DedicatedServerURL, ErrorMessage);
}

//Add inventory logic
void USFGameInstanceSubsystem::UpdatePlayerInventory(const FString& PlayerID, const TArray<USFItemBase*>& Inventory)
{
	PlayerInventories.Emplace(PlayerID, Inventory);
	UE_LOG(LogTemp, Log, TEXT("Updated inventory for Player: %s, Item Count: %d"), *PlayerID, Inventory.Num());
}

void USFGameInstanceSubsystem::UpdatePlayerEquipment(const FString& PlayerID, USFEquipableBase* Common, USFEquipableBase* Exclusive, USFEquipableBase* Cosmetic)
{
	TArray<USFEquipableBase*> Equipment;
	Equipment.Add(Common);
	Equipment.Add(Exclusive);
	Equipment.Add(Cosmetic);
	PlayerEquipments.Emplace(PlayerID, Equipment);
	UE_LOG(LogTemp, Log, TEXT("Updated equipment for Player: %s"), *PlayerID);
}

TArray<USFItemBase*> USFGameInstanceSubsystem::GetPlayerInventory(const FString& PlayerID) const
{
	if (PlayerInventories.Contains(PlayerID))
	{
		return PlayerInventories[PlayerID];
	}
	return TArray<USFItemBase*>();
}

const USFEquipableBase* USFGameInstanceSubsystem::GetPlayerEquippedItem(const FString& PlayerID, SFEquipSlot Slot) const
{
	if (PlayerEquipments.Contains(PlayerID))
	{
		const TArray<USFEquipableBase*>& Equipment = PlayerEquipments[PlayerID];
		switch (Slot)
		{
		case SFEquipSlot::CommonSlot:
			return Equipment.IsValidIndex(0) ? Equipment[0] : nullptr;
		case SFEquipSlot::ExclusiveSlot:
			return Equipment.IsValidIndex(1) ? Equipment[1] : nullptr;
		case SFEquipSlot::CosmeticSlot:
			return Equipment.IsValidIndex(2) ? Equipment[2] : nullptr;
		default:
			return nullptr;
		}
	}
	return nullptr;
}

//Inventory logic when(no character)
void USFGameInstanceSubsystem::AddPendingShopPurchase(const FString& PlayerID, TSubclassOf<class USFItemBase> ItemClass)
{
	if (!PendingShopPurchases.Contains(PlayerID))
	{
		PendingShopPurchases.Add(PlayerID, TArray<TSubclassOf<class USFItemBase>>());
		UE_LOG(LogTemp,Warning,TEXT("Pending Purchase Array Added"))
	}
	PendingShopPurchases[PlayerID].Add(ItemClass);
	UE_LOG(LogTemp, Log, TEXT("Added pending shop purchase for Player: %s, Item: %s"), *PlayerID, *ItemClass->GetName());
}

TArray<TSubclassOf<class USFItemBase>> USFGameInstanceSubsystem::GetPendingShopPurchases(const FString& PlayerID) const
{
	if (PendingShopPurchases.Contains(PlayerID))
	{
		return PendingShopPurchases[PlayerID];
	}
	return TArray<TSubclassOf<class USFItemBase>>();
}

void USFGameInstanceSubsystem::ClearPendingShopPurchases(const FString& PlayerID)
{
	PendingShopPurchases.Remove(PlayerID);
	UE_LOG(LogTemp, Log, TEXT("Cleared pending shop purchases for Player: %s"), *PlayerID);
}