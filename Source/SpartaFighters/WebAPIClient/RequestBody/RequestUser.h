#pragma once
#include "CoreMinimal.h"
#include "RequestUser.generated.h"

USTRUCT(Atomic, BlueprintType)
struct SPARTAFIGHTERS_API FRequestCreateAccount
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    FString Account;

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    FString Password;
};


USTRUCT(Atomic, BlueprintType)
struct SPARTAFIGHTERS_API FRequestLogIn
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    FString Account;

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    FString Password;
};

USTRUCT(Atomic, BlueprintType)
struct SPARTAFIGHTERS_API FRequestLogOut
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    FString SessionId;
};
