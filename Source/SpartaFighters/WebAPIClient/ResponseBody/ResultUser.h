#pragma once

#include "CoreMinimal.h"
#include "ResultUser.generated.h"

USTRUCT(BlueprintType)
struct FUser
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    int32 UserId;

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    FString Account;

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    FString Password;
};

USTRUCT(BlueprintType)
struct FUserResult
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    TArray<FUser> Data;
};

USTRUCT(BlueprintType)
struct FResultCreateAccount
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    int32 ResultCode;

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    FString ResultMessage;

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    FString Data;
};

USTRUCT(BlueprintType)
struct FResultLogInData
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    FString SessionId;
};

USTRUCT(BlueprintType)
struct FResultLogIn
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    int32 ResultCode;

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    FString ResultMessage;

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    FResultLogInData Data;
};
