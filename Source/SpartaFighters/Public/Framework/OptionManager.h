

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Sound/SoundMix.h"
#include "Sound/SoundClass.h"
#include "OptionManager.generated.h"

UENUM(BlueprintType)
enum class EScreenMode : uint8
{
	FullScreen UMETA(DisplayName = "FullScreen"),
	WindowedFullScreen UMETA(DisplayName = "WindowedFullScreen"),
	Window UMETA(DisplayName = "Window")
};

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

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	EScreenMode CurrentMode;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FIntPoint CurrentResolution;
protected:
	float backgroundSound = 0;
	float effectSound = 0;


public:

	UFUNCTION()
	void BGSoundValue(float Value);
	UFUNCTION()
	void EFSoundValue(float Value);
	UFUNCTION()
	void ChangeScreen(EScreenMode Mode);
	UFUNCTION()
	void ChangeResolution(FIntPoint Resolution);
};
