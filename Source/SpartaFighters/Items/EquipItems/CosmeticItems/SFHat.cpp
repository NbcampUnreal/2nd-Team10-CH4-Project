


#include "Items/EquipItems/CosmeticItems/SFHat.h"
#include "Components/StaticMeshComponent.h"

USFHat::USFHat()
{
	ItemType = EItemType::Cosmetic;
	ItemName = "Hat";
	ItemDescription = FText::FromString(TEXT("Well designed hat."));
	HatMesh = TSoftObjectPtr<UStaticMesh>(FSoftObjectPath(TEXT("/Game/PlatformFighterKit/Assets/Mesh/Hats/SM_Tophat_01"))); // ���� ���۷��� ���� (��δ� ���� ���� ��η� ����)
}


void USFHat::ApplyCosmetic_Implementation(AActor* Owner)
{
	Super::ApplyCosmetic_Implementation(Owner);
	UE_LOG(LogTemp, Warning, TEXT("%s attatched!"), *ItemName.ToString());


	if (Owner)
	{
		FName AttachPoint = "head"; // ĳ���� �޽��� �Ӹ� �� �̸� (���� ������Ʈ�� �°� ����)
		UStaticMeshComponent* HatComponent = NewObject<UStaticMeshComponent>(Owner, FName(*FString::Printf(TEXT("%s_Hat"), *ItemName.ToString())));
		if (HatComponent && HatMesh.IsValid()) // HatMesh�� �ε��Ǿ����� Ȯ��
		{
			HatComponent->SetStaticMesh(HatMesh.Get()); // Get() �Լ��� ���� UStaticMesh ���
			HatComponent->SetupAttachment(Owner->GetRootComponent(), AttachPoint);
			HatComponent->RegisterComponent();
			// �ʿ��ϴٸ� Tag ������ �����Ͽ� ���� �� ã�� �� �ֵ��� ��
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
	UE_LOG(LogTemp, Warning, TEXT("%s ����!"), *ItemName.ToString());

	// ����: �������� �Ӹ��� ������ ���� �޽� ����
	if (Owner)
	{
		TArray<UStaticMeshComponent*> Components;
		Owner->GetComponents<UStaticMeshComponent>(Components);
		for (UStaticMeshComponent* Component : Components)
		{
			if (Component && Component->GetName().Contains(ItemName.ToString() + "_Hat"))
			{
				Component->DestroyComponent();
				break; // �ϳ��� ���ڸ� �ִٰ� ����
			}
		}
	}
}

void USFHat::SetHatMesh(TSoftObjectPtr<UStaticMesh>Mesh)
{
	HatMesh = Mesh;
}