#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "DefaultGameIni.generated.h"


UCLASS()
class SPARTAFIGHTERS_API UDefaultGameIni : public UObject
{
	GENERATED_BODY()

public:
	static UDefaultGameIni* GetInstance();

	const FText& GetVersion() const { return Version; }
	bool GetShowLog() const { return bShowLog; }

	bool GetWebAPIUse() const { return bWebAPIUse; }
	const FString& GetWebAPIServerURL() const { return WebAPIServerURL; }

private:
	UDefaultGameIni();
	static UDefaultGameIni* Instance;

	void LoadConfig();
	
	// System
	FText Version;
	bool bShowLog;

	// Web API
	bool bWebAPIUse;
	FString WebAPIServerURL;
};
