#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "SFGameStateBase.generated.h"

UCLASS()
class SPARTAFIGHTERS_API ASFGameStateBase : public AGameStateBase
{
	GENERATED_BODY()

public:
	// 선택된 모드와 맵 정보
	/*UPROPERTY(Replicated)
	EGameModeType SelectedGameMode;

	UPROPERTY(Replicated)
	int32 SelectedMapID;*/

	// virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};