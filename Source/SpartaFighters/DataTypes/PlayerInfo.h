#pragma once

#include "CoreMinimal.h"
#include "Net/Serialization/FastArraySerializer.h"
#include "PlayerInfo.generated.h"

USTRUCT(BlueprintType)
struct FPlayerInfo : public FFastArraySerializerItem
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString PlayerID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bIsReady = false;
    // Send the texture path and let the client load
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString CharacterTexturePath;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString SelectedCharacterName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TSubclassOf<APawn> SelectedCharacterClass;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FString> EquippedItems;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 CurrentRoomID = -1;

    void PreReplicatedRemove(const struct FPlayerInfoArray& InArraySerializer);
    void PostReplicatedAdd(const struct FPlayerInfoArray& InArraySerializer);
    void PostReplicatedChange(const struct FPlayerInfoArray& InArraySerializer);
};

USTRUCT(BlueprintType)
struct FPlayerInfoArray : public FFastArraySerializer
{
    GENERATED_BODY()

    UPROPERTY()
    TArray<FPlayerInfo> Items;

    void AddPlayer(const FPlayerInfo& NewPlayer);

    void RemovePlayer(const FString& PlayerID);

    bool NetDeltaSerialize(FNetDeltaSerializeInfo& DeltaParms);
};

template<>
struct TStructOpsTypeTraits<FPlayerInfoArray> : public TStructOpsTypeTraitsBase2<FPlayerInfoArray>
{
    enum
    {
        WithNetDeltaSerializer = true,
    };
};