#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Items/SFItemBase.h"
#include "SFInventoryInteractionComponent.generated.h"

class USFInventoryComponent;
class USFGameInstanceSubsystem;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SPARTAFIGHTERS_API USFInventoryInteractionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	USFInventoryInteractionComponent();

	virtual void BeginPlay() override;

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_GetPendingPurchases(const FString& PlayerID);
	virtual void Server_GetPendingPurchases_Implementation(const FString& PlayerID);
	virtual bool Server_GetPendingPurchases_Validate(const FString& PlayerID);

	UFUNCTION(Client, Reliable)
	void Client_ReceivePendingPurchases(const TArray<TSubclassOf<class USFItemBase>>& PendingPurchases);
	virtual void Client_ReceivePendingPurchases_Implementation(const TArray<TSubclassOf<class USFItemBase>>& PendingPurchases);

public:
	USFGameInstanceSubsystem* GetGameInstanceSubsystem();
private:
	USFInventoryComponent* InventoryComponent = nullptr;
		
};
