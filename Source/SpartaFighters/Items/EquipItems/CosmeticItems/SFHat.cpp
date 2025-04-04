


#include "Items/EquipItems/CosmeticItems/SFHat.h"
#include "Components/StaticMeshComponent.h"

USFHat::USFHat()
{
	ItemType = EItemType::Cosmetic;
	ItemName = "Hat";
	ItemDescription = FText::FromString(TEXT("Well designed hat."));
	HatMesh = TSoftObjectPtr<UStaticMesh>(FSoftObjectPath(TEXT("/Game/PlatformFighterKit/Assets/Mesh/Hats/SM_Tophat_01"))); // 에셋 레퍼런스 설정 (경로는 실제 에셋 경로로 변경)
}


void USFHat::ApplyCosmetic_Implementation(AActor* Owner)
{
	Super::ApplyCosmetic_Implementation(Owner);
	UE_LOG(LogTemp, Warning, TEXT("%s attatched!"), *ItemName.ToString());


	if (Owner)
	{
		FName AttachPoint = "head"; 
		UStaticMeshComponent* HatComponent = NewObject<UStaticMeshComponent>(Owner, FName(*FString::Printf(TEXT("%s_Hat"), *ItemName.ToString())));
		if (HatComponent && HatMesh.IsValid()) 
		{
			HatComponent->SetStaticMesh(HatMesh.Get()); 
			HatComponent->SetupAttachment(Owner->GetRootComponent(), AttachPoint);
			HatComponent->RegisterComponent();
			
		}
		else if (!HatMesh.IsValid() && HatMesh.GetAssetName() != "")
		{
			UE_LOG(LogTemp, Warning, TEXT("%s equipment failed: HatMesh is not loaded. asset name: %s"), *ItemName.ToString(), *HatMesh.GetAssetName());
		
		}
		else if (HatMesh.GetAssetName() == "")
		{
			UE_LOG(LogTemp, Warning, TEXT("%s equipment failed: HatMesh is not setted."), *ItemName.ToString());
		}
	}
}

void USFHat::RemoveCosmetic_Implementation(AActor* Owner)
{
	Super::RemoveCosmetic_Implementation(Owner);
	UE_LOG(LogTemp, Warning, TEXT("%s Remove!"), *ItemName.ToString());

	
	if (Owner)
	{
		TArray<UStaticMeshComponent*> Components;
		Owner->GetComponents<UStaticMeshComponent>(Components);
		for (UStaticMeshComponent* Component : Components)
		{
			if (Component && Component->GetName().Contains(ItemName.ToString() + "_Hat"))
			{
				Component->DestroyComponent();
				break; 
			}
		}
	}
}

void USFHat::SetHatMesh(TSoftObjectPtr<UStaticMesh>Mesh)
{
	HatMesh = Mesh;
}