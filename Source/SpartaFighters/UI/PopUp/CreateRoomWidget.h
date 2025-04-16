#pragma once

#include "CoreMinimal.h"
#include "UI/PopUp/SelectPopUpBase.h"
#include "DataTypes/GameModeType.h"
#include "CreateRoomWidget.generated.h"

class UTextBlock;
class UGameModeSelectionWidget;
class UPlayerCountSelectionWidget;
class UItemActivationSelectionWidget;
class URoomWidget;

UCLASS()
class SPARTAFIGHTERS_API UCreateRoomWidget : public USelectPopUpBase
{
	GENERATED_BODY()
	
protected:
	//virtual void NativeConstruct() override;

};
