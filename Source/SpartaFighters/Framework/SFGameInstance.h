#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "DataTable/MapInfoRow.h"
#include "SFGameInstance.generated.h"

class UUIManagerSettings;

UCLASS()
class SPARTAFIGHTERS_API USFGameInstance : public UGameInstance
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category = "UI")
	UUIManagerSettings* UISettings;
	
public:
	UUIManagerSettings* GetUISettings() const { return UISettings; }
	
	FMapInfoRow CurrentMapInfoRow;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map Data")
	UDataTable* MapDataTable;

	void LoadMapData();
};
