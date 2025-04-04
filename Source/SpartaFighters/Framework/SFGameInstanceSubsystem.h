#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
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

UENUM()
enum class EGameMode : uint8
{
	Cooperative UMETA(DisplayName = "Cooperative"),
	Battle      UMETA(DisplayName = "Battle"),
	Single      UMETA(DisplayName = "Single")
};

class ASFGameModeBase;

UCLASS()
class SPARTAFIGHTERS_API USFGameInstanceSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
private:
	EGameState CurrentGameState;
	EGameMode CurrentGameMode;
	UPROPERTY()
	ASFGameModeBase* CurrentGameModeInstance;

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	EGameState GetCurrentGameState() const { return CurrentGameState; }
	void SetCurrentGameState(EGameState NewGameState);
	EGameMode GetCurrentGameMode() const { return CurrentGameMode; }
	void SwitchGameMode(EGameMode NewGameMode);
};
