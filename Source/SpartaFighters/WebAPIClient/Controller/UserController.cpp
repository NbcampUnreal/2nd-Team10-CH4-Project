#include "WebAPIClient/Controller/UserController.h"
#include "WebAPIClient/WebAPIClient.h"
#include "Common/DefaultGameIni.h"

UUserController* UUserController::Instance = nullptr;

UUserController::UUserController()
{
    if (!WebAPIClient)
    {
        WebAPIClient = NewObject<UWebAPIClient>();
        WebAPIClient->AddToRoot();
    }
}

UUserController* UUserController::GetInstance()
{
    if (!Instance)
    {
        //FScopeLock Lock(&Mutex); // 멀티스레드에서 안전한 접근 보장
        if (!Instance) // 다시 한 번 체크 (Double-Checked Locking)
        {
            Instance = NewObject<UUserController>();
            Instance->AddToRoot(); // GC에서 삭제되지 않도록 방지

            Instance->Init();
        }
    }
    return Instance;
}

void UUserController::Init()
{
    // TODO : Config에 저장된 서버 URL 받아와서 맵핑해주기
    //auto config = UDefaultGameIni::GetInstance();
    //if (IsValid(config))
    //{
    //    FString url = config->GetWebAPIServerURL() + "/User";
    //}
}

void UUserController::GetAllUsers(TFunction<void(TArray<FUser>)> Callback)
{
    WebAPIClient->GetAsync<FUser>(URL, Callback);
}

void UUserController::CreateAccount(const FRequestCreateAccount& Request, TFunction<void(TSharedPtr<FResultCreateAccount>)> Callback)
{
    WebAPIClient->PostAsync<FRequestCreateAccount, FResultCreateAccount>(URL + "/CreateAccount", Request, Callback);
}

void UUserController::LogIn(const FRequestLogIn& Request, TFunction<void(TSharedPtr<FResultLogIn>)> Callback)
{
    WebAPIClient->PostAsync<FRequestLogIn, FResultLogIn>(URL + "/LogIn", Request, Callback);
}
