#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "WebAPIClient/RequestBody/RequestUser.h"
#include "WebAPIClient/ResponseBody/ResultUser.h"
#include "UserController.generated.h"

class UWebAPIClient;

UCLASS()
class SPARTAFIGHTERS_API UUserController : public UObject
{
	GENERATED_BODY()

public:
    UUserController();

    static UUserController* GetInstance();

    void GetAllUsers(TFunction<void(TArray<FUser>)> Callback);
    void CreateAccount(const FRequestCreateAccount& Request, TFunction<void(TSharedPtr<FResultCreateAccount>)> Callback);
    void LogIn(const FRequestLogIn& Request, TFunction<void(TSharedPtr<FResultLogIn>)> Callback);


private:
    static UUserController* Instance;
    UWebAPIClient* WebAPIClient;

    FString URL = TEXT("http://localhost:5000/api/User");

    //static FCriticalSection Mutex; // 멀티스레드 안전성 보장
};
