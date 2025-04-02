#pragma once

#include "CoreMinimal.h"

template <typename TData>
struct FResponseBody
{
    int32 ResultCode;
    FString ResultMessage;
    TData Data;
};