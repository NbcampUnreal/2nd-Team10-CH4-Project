#include "DataTypes/PlayerInfo.h"
#include "Net/UnrealNetwork.h"

void FPlayerInfoArray::AddPlayer(const FPlayerInfo& NewPlayer)
{
    Items.Add(NewPlayer);
    MarkItemDirty(Items.Last());
}

void FPlayerInfoArray::RemovePlayer(const FString& PlayerID)
{
    for (int32 i = 0; i < Items.Num(); ++i)
    {
        if (Items[i].PlayerID == PlayerID)
        {
            Items.RemoveAt(i);
            MarkArrayDirty();
            break;
        }
    }
}

bool FPlayerInfoArray::NetDeltaSerialize(FNetDeltaSerializeInfo& DeltaParms)
{
    return FFastArraySerializer::FastArrayDeltaSerialize<FPlayerInfo, FPlayerInfoArray>(Items, DeltaParms, *this);
}

void FPlayerInfo::PreReplicatedRemove(const FPlayerInfoArray& InArraySerializer)
{
    UE_LOG(LogTemp, Log, TEXT("[FastArray] PreRemove Player: %s"), *PlayerID);
}

void FPlayerInfo::PostReplicatedAdd(const FPlayerInfoArray& InArraySerializer)
{
    UE_LOG(LogTemp, Log, TEXT("[FastArray] PostAdd Player: %s"), *PlayerID);
}

void FPlayerInfo::PostReplicatedChange(const FPlayerInfoArray& InArraySerializer)
{
    UE_LOG(LogTemp, Log, TEXT("[FastArray] PostChange Player: %s"), *PlayerID);
}
