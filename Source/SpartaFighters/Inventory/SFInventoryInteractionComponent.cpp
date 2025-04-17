#include "Inventory/SFInventoryInteractionComponent.h"
#include "Inventory/SFInventoryComponent.h"
#include "Framework/SFGameInstanceSubsystem.h"
#include "Net/UnrealNetwork.h"
#include "Framework/SFPlayerState.h"
#include "Character/SFCharacter.h"

USFInventoryInteractionComponent::USFInventoryInteractionComponent()
{
	
	PrimaryComponentTick.bCanEverTick = true;

}

void USFInventoryInteractionComponent::BeginPlay()
{
	Super::BeginPlay();

	AActor* Owner = GetOwner();
	if (Owner)
	{
		InventoryComponent = Owner->FindComponentByClass<USFInventoryComponent>();
	}
}

void USFInventoryInteractionComponent::Server_GetPendingPurchases_Implementation(const FString& PlayerID)
{
	AActor* Owner = GetOwner();
	if (Owner&&Owner->HasAuthority())
	{
		USFGameInstanceSubsystem* GameInstanceSubsystem = GetGameInstanceSubsystem();
		if (GameInstanceSubsystem)
		{
			TArray<TSubclassOf<class USFItemBase>> PendingPurchases = GameInstanceSubsystem->GetPendingShopPurchases(PlayerID);
			Client_ReceivePendingPurchases(PendingPurchases);
			
		}
	}
}

bool USFInventoryInteractionComponent::Server_GetPendingPurchases_Validate(const FString& PlayerID)
{
	return !PlayerID.IsEmpty();
}

//void USFInventoryInteractionComponent::Client_ReceivePendingPurchases_Implementation(const TArray<TSubclassOf<class USFItemBase>>& PendingPurchases)
//{
//	if (InventoryComponent)
//	{
//		for (const auto& ItemClass : PendingPurchases)
//		{
//			InventoryComponent->Server_AddItemByClass(ItemClass);
//		}
//	}
//}
void USFInventoryInteractionComponent::Client_ReceivePendingPurchases_Implementation(const TArray<TSubclassOf<class USFItemBase>>& PendingPurchases)
{
	ASFCharacter* OwnerCharacter = Cast<ASFCharacter>(GetOwner());
	if (OwnerCharacter)
	{
		USFInventoryComponent* InventoryComp = OwnerCharacter->FindComponentByClass<USFInventoryComponent>();
		if (InventoryComp)
		{
			UE_LOG(LogTemp, Warning, TEXT("Client received pending purchases. Count: %d"), PendingPurchases.Num());
			for (const auto& ItemClass : PendingPurchases)
			{
				UE_LOG(LogTemp, Warning, TEXT("Client attempting to add item class: %s to inventory."), *ItemClass->GetName());
				InventoryComp->Server_AddItemByClass(ItemClass); 
				UE_LOG(LogTemp, Warning, TEXT("Client added item to inventory."));
			}

			USFGameInstanceSubsystem* GameInstanceSubsystem = GetGameInstanceSubsystem();
			if (GameInstanceSubsystem)
			{
				if (ASFPlayerState* PS = OwnerCharacter->GetPlayerState<ASFPlayerState>())
				{
					GameInstanceSubsystem->ClearPendingShopPurchases(PS->GetUniqueID());
					UE_LOG(LogTemp, Warning, TEXT("Client cleared pending purchases on GameInstanceSubsystem."));
				}
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("InventoryComponent is null on client!"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Owner character is null on client!"));
	}
}

USFGameInstanceSubsystem* USFInventoryInteractionComponent::GetGameInstanceSubsystem()
{
	UGameInstance* GameInstance = GetWorld()->GetGameInstance();
	if (GameInstance)
	{
		return GameInstance->GetSubsystem<USFGameInstanceSubsystem>();
	}
	return nullptr;
}