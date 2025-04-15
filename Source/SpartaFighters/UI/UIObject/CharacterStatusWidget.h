#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CharacterStatusWidget.generated.h"

class UTextBlock;
class UProgressBar;
class UImage;

UCLASS()
class SPARTAFIGHTERS_API UCharacterStatusWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
    void SetPlayerName(const FString& Name);
    void BindToCharacter(APawn* TargetCharacter);
    void UpdateHPBar(float CurrentHP, float MaxHP);

    UFUNCTION()
    void OnCharacterHPChanged(AActor* AffectedActor, float NewHP);

    FString GetPlayerName() const;

protected:
	virtual void NativeConstruct() override;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* PlayerName;
    UPROPERTY(meta = (BindWidget))
    UProgressBar* KnockBackBar;
    UPROPERTY(meta = (BindWidget))
    UProgressBar* HPBar;
    UPROPERTY(meta = (BindWidget))
    UProgressBar* MPBar;
    UPROPERTY(meta = (BindWidget))
    UImage* CharacterIcon;
};
