#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "CharacterDataRow.generated.h"

USTRUCT(BlueprintType)
struct FCharacterDataRow : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName CharacterName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<APawn> CharacterClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* Thumbnail;
};
