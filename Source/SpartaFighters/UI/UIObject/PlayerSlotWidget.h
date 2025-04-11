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
	UPROPERTY()
	FString SlotPlayerID;

	UFUNCTION()
	void SetupPlayerSlot(const FString& PlayerName, const FString& CharacterPreviewPath, bool bIsReady);
	UFUNCTION()
	void SetReadyState(bool bIsReady);
	UFUNCTION()
	void SetEmpty();
	UFUNCTION()
	void UpdateRoomOwner(const FString& OwnerPlayerID);
protected:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* PlayerNameText;
	UPROPERTY(meta = (BindWidget))
	UImage* CharacterPreviewImage;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* ReadyStateText;
	UPROPERTY(meta = (BindWidget))
	class UBorder* SlotBorder;
	UPROPERTY(meta = (BindWidget))
	UImage* HostIcon;
};
