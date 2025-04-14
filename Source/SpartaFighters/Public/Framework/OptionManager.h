

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Sound/SoundMix.h"
#include "Sound/SoundClass.h"
#include "OptionManager.generated.h"


UCLASS()
class SPARTAFIGHTERS_API UOptionManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UOptionManager();
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	USoundMix* MainSoundMix;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	USoundClass* SC_BackGround;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	USoundClass* SC_Effect;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UUserWidget* SoundWidget;
protected:
	float backgroundSound = 0;
	float effectSound = 0;


public:

	UFUNCTION()
	void BGSoundValue(float Value);
	UFUNCTION()
	void EFSoundValue(float Value);
	
};
