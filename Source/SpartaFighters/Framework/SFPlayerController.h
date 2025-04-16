#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SFPlayerController.generated.h"

class UInputAction;
class UInputMappingContext;
class ASFCharacter;
class USFInventoryComponent;
class USFGameInstanceSubsystem;

UCLASS()
class SPARTAFIGHTERS_API ASFPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	ASFPlayerController();

	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputMappingContext* InputMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* DodgeAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* AttackAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* SkillAttackAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* GuardAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* CrouchAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* InteractionAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* SettingAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* InventoryCheckAction;
public:
	void AddMappingContext();
	// Move to SFLobbyPlayerController.cpp
	//void InitUI();

public:
	UFUNCTION(Server, Reliable)
	void Server_RequestSpawnCharacter();
	UFUNCTION(Client, Reliable)
	void Client_StartHUDUpdate();
	UFUNCTION(Client, Reliable)
	void Client_EndBattle();
	UFUNCTION(Client, Reliable)
	void Client_EndReturnToLobbyTimer();
	UFUNCTION(Client, Reliable)
	void Client_TravelToLobby();

	void OnPossess(APawn* InPawn) override;

private:
	void SetupCharacterInventory();

	UFUNCTION(BlueprintPure, Category = "Game Instance")
	USFGameInstanceSubsystem* GetGameInstanceSubsystem() const;
};
