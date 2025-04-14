#pragma once

#include "CoreMinimal.h"
#include "Http.h"
//#include "Json.h"
//#include "HttpModule.h"
//#include "Interfaces/IHttpRequest.h"
//#include "Interfaces/IHttpResponse.h"
//#include "JsonUtilities.h"
#include "JsonObjectConverter.h"
#include "WebAPIClient.generated.h"

UCLASS()
class SPARTAFIGHTERS_API UWebAPIClient : public UObject
{
	GENERATED_BODY()
public:
    template <typename TResultType>
    void GetAsync(const FString& Url, TFunction<void(TArray<TResultType>)> Callback);

    //template <typename TResultType>
    //void GetAsync(const FString& Url, TFunction<void(TSharedPtr<TResultType>)> Callback);

	template <typename TRequestValue, typename TResultType>
	void PostAsync(const FString& Url, const TRequestValue& RequestValue, TFunction<void(TSharedPtr<TResultType>)> Callback);

    template <typename TRequestValue, typename TResultType>
    void PutAsync(const FString& Url, const TRequestValue& RequestValue, TFunction<void(TSharedPtr<TResultType>)> Callback);

    template <typename TResultType>
    void DeleteAsync(const FString& Url, const int32& Id, TFunction<void(TSharedPtr<TResultType>)> Callback);
};


template<typename TResultType>
void UWebAPIClient::GetAsync(const FString& Url, TFunction<void(TArray<TResultType>)> Callback)
{
    TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = FHttpModule::Get().CreateRequest();
    Request->SetURL(Url);
    Request->SetVerb(TEXT("GET"));
    Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));

    Request->OnProcessRequestComplete().BindLambda([Callback](FHttpRequestPtr Req, FHttpResponsePtr Res, bool bSuccess)
        {
            if (!bSuccess || !Res.IsValid())
            {
                UE_LOG(LogTemp, Error, TEXT("GET request failed."));
                Callback(TArray<TResultType>());
                return;
            }

            FString ResponseString = Res->GetContentAsString();
            TArray<TResultType> ResponseData;

            if (!FJsonObjectConverter::JsonArrayStringToUStruct(ResponseString, &ResponseData, 0, 0))
            {
                UE_LOG(LogTemp, Error, TEXT("Failed to deserialize GET response."));
                Callback(TArray<TResultType>());
                return;
            }

            Callback(ResponseData);
        });

    Request->ProcessRequest();
}

// GET
//template<typename TResultType>
//void UWebAPIClient::GetAsync(const FString& Url, TFunction<void(TSharedPtr<TResultType>)> Callback)
//{
//    TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = FHttpModule::Get().CreateRequest();
//    Request->SetURL(Url);
//    Request->SetVerb(TEXT("GET"));
//    Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
//
//    Request->OnProcessRequestComplete().BindLambda([Callback](FHttpRequestPtr Req, FHttpResponsePtr Res, bool bSuccess)
//        {
//            if (!bSuccess || !Res.IsValid())
//            {
//                UE_LOG(LogTemp, Error, TEXT("GET request failed."));
//                Callback(nullptr);
//                return;
//            }
//
//            FString ResponseString = Res->GetContentAsString();
//            TSharedPtr<TResultType> ResponseData = MakeShareable(new TResultType());
//
//            if (!FJsonObjectConverter::JsonObjectStringToUStruct(ResponseString, ResponseData.Get(), 0, 0))
//            {
//                UE_LOG(LogTemp, Error, TEXT("Failed to deserialize GET response."));
//                Callback(nullptr);
//                return;
//            }
//
//            Callback(ResponseData);
//        });
//
//    Request->ProcessRequest();
//}

// POST
template<typename TRequestValue, typename TResultType>
void UWebAPIClient::PostAsync(const FString& Url, const TRequestValue& RequestValue, TFunction<void(TSharedPtr<TResultType>)> Callback)
{
    FString JsonString;
    if (!FJsonObjectConverter::UStructToJsonObjectString(RequestValue, JsonString))
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to serialize request body."));
        Callback(nullptr);
        return;
    }

    TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = FHttpModule::Get().CreateRequest();
    Request->SetURL(Url);
    Request->SetVerb(TEXT("POST"));
    Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
    Request->SetContentAsString(JsonString);

    Request->OnProcessRequestComplete().BindLambda([Callback](FHttpRequestPtr Req, FHttpResponsePtr Res, bool bSuccess)
        {
            if (!bSuccess || !Res.IsValid())
            {
                UE_LOG(LogTemp, Error, TEXT("Request failed."));
                Callback(nullptr);
                return;
            }

            FString ResponseString = Res->GetContentAsString();
            TSharedPtr<TResultType> ResponseData = MakeShareable(new TResultType());

            if (!FJsonObjectConverter::JsonObjectStringToUStruct(ResponseString, ResponseData.Get(), 0, 0))
            {
                UE_LOG(LogTemp, Error, TEXT("Failed to deserialize response."));
                Callback(nullptr);
                return;
            }

            Callback(ResponseData);
        });

    Request->ProcessRequest();
}

// PUT
template<typename TRequestValue, typename TResultType>
void UWebAPIClient::PutAsync(const FString& Url, const TRequestValue& RequestValue, TFunction<void(TSharedPtr<TResultType>)> Callback)
{
    FString JsonString;
    if (!FJsonObjectConverter::UStructToJsonObjectString(RequestValue, JsonString))
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to serialize PUT request body."));
        Callback(nullptr);
        return;
    }

    TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = FHttpModule::Get().CreateRequest();
    Request->SetURL(Url);
    Request->SetVerb(TEXT("PUT"));
    Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
    Request->SetContentAsString(JsonString);

    Request->OnProcessRequestComplete().BindLambda([Callback](FHttpRequestPtr Req, FHttpResponsePtr Res, bool bSuccess)
        {
            if (!bSuccess || !Res.IsValid())
            {
                UE_LOG(LogTemp, Error, TEXT("PUT request failed."));
                Callback(nullptr);
                return;
            }

            FString ResponseString = Res->GetContentAsString();
            TSharedPtr<TResultType> ResponseData = MakeShareable(new TResultType());

            if (!FJsonObjectConverter::JsonObjectStringToUStruct(ResponseString, ResponseData.Get(), 0, 0))
            {
                UE_LOG(LogTemp, Error, TEXT("Failed to deserialize PUT response."));
                Callback(nullptr);
                return;
            }

            Callback(ResponseData);
        });

    Request->ProcessRequest();
}

// DELETE
template<typename TResultType>
void UWebAPIClient::DeleteAsync(const FString& Url, const int32& Id, TFunction<void(TSharedPtr<TResultType>)> Callback)
{
    TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = FHttpModule::Get().CreateRequest();
    Request->SetURL(Url + TEXT("/") + FString::FromInt(Id));
    Request->SetVerb(TEXT("DELETE"));
    Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));

    Request->OnProcessRequestComplete().BindLambda([Callback](FHttpRequestPtr Req, FHttpResponsePtr Res, bool bSuccess)
        {
            if (!bSuccess || !Res.IsValid())
            {
                UE_LOG(LogTemp, Error, TEXT("DELETE request failed."));
                Callback(nullptr);
                return;
            }
            Callback(MakeShareable(new TResultType()));
        });

    Request->ProcessRequest();
}
