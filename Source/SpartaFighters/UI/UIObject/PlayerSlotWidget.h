#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerSlotWidget.generated.h"

class UTextBlock;
class UImage;

UCLASS()
class SPARTAFIGHTERS_API UPlayerSlotWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void SetupPlayerSlot(const FString& PlayerName, UTexture2D* CharacterPreview, bool bIsReady);
	void ClearSlot;

protected:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* PlayerNameText;
	UPROPERTY(meta = (BindWidget))
	UImage* CharacterPreviewImage;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* ReadyStatusText;
};
