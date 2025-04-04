


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
		FName AttachPoint = "head"; // 캐릭터 메시의 머리 본 이름 (실제 프로젝트에 맞게 변경)
		UStaticMeshComponent* HatComponent = NewObject<UStaticMeshComponent>(Owner, FName(*FString::Printf(TEXT("%s_Hat"), *ItemName.ToString())));
		if (HatComponent && HatMesh.IsValid()) // HatMesh가 로딩되었는지 확인
		{
			HatComponent->SetStaticMesh(HatMesh.Get()); // Get() 함수로 실제 UStaticMesh 얻기
			HatComponent->SetupAttachment(Owner->GetRootComponent(), AttachPoint);
			HatComponent->RegisterComponent();
			// 필요하다면 Tag 등으로 관리하여 해제 시 찾을 수 있도록 함
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
	UE_LOG(LogTemp, Warning, TEXT("%s 해제!"), *ItemName.ToString());

	// 예시: 소유자의 머리에 부착된 모자 메시 제거
	if (Owner)
	{
		TArray<UStaticMeshComponent*> Components;
		Owner->GetComponents<UStaticMeshComponent>(Components);
		for (UStaticMeshComponent* Component : Components)
		{
			if (Component && Component->GetName().Contains(ItemName.ToString() + "_Hat"))
			{
				Component->DestroyComponent();
				break; // 하나의 모자만 있다고 가정
			}
		}
	}
}

void USFHat::SetHatMesh(TSoftObjectPtr<UStaticMesh>Mesh)
{
	HatMesh = Mesh;
}