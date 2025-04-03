#include "Common/DefaultGameIni.h"
#include "Misc/ConfigCacheIni.h"

UDefaultGameIni* UDefaultGameIni::Instance = nullptr;

UDefaultGameIni* UDefaultGameIni::GetInstance()
{
	if (!Instance)
	{
		Instance = NewObject<UDefaultGameIni>();
		Instance->AddToRoot(); // Prevent GC
		Instance->LoadConfig();
	}
	return Instance;
}

UDefaultGameIni::UDefaultGameIni() {}

void UDefaultGameIni::LoadConfig()
{
	if (GConfig)
	{
		GConfig->GetText(TEXT("System"), TEXT("Version"), Version, GGameIni);
		GConfig->GetBool(TEXT("System"), TEXT("ShowLog"), bShowLog, GGameIni);
		GConfig->GetBool(TEXT("WebAPI"), TEXT("WebAPIUse"), bWebAPIUse, GGameIni);
		GConfig->GetString(TEXT("WebAPI"), TEXT("WebAPIServerURL"), WebAPIServerURL, GGameIni);
	}
}