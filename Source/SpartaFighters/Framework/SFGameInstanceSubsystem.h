#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "DataTypes/GameModeType.h"
#include "DataTable/MapInfoRow.h"
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
	void ChangeLevelByMapID(int32 MapID);

	UUIManager* GetUIManager() const { return UIManager; }
	
};
