#pragma once

#include "CoreMinimal.h"
#include "PlayerInfo.generated.h"

USTRUCT(BlueprintType)
struct FPlayerInfo
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString PlayerID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bIsReady = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UTexture2D* CharacterTexture = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString SelectedCharacterName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TSubclassOf<APawn> SelectedCharacterClass;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FString> EquippedItems;
};
