#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "DataTable/MapInfoRow.h"
#include "DataTable/CharacterDataRow.h"
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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	UDataTable* MapDataTable;
	void LoadMapData();

	/*FCharacterDataRow CurrentCharacterInfoRow;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	UDataTable* CharacterDataTable;

	void LoadCharacterData();*/
};
