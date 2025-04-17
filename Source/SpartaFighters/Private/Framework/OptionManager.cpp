
#include "Framework/OptionManager.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/GameUserSettings.h"

UOptionManager::UOptionManager()
{
	CurrentResolution = {1920,1080};
	CurrentMode = EScreenMode::FullScreen;
	backgroundSound = 0.5f;
	effectSound = 0.5f;
	ConstructorHelpers::FObjectFinder<USoundMix>
		SoundMix(TEXT("/Script/Engine.SoundMix'/Game/PlatformFighterKit/Blueprints/Option/SoundOption/MainSoundMix.MainSoundMix'"));
	ConstructorHelpers::FObjectFinder<USoundClass>
		BackGround(TEXT("/Script/Engine.SoundClass'/Game/PlatformFighterKit/Blueprints/Option/SoundOption/SC_BackGround.SC_BackGround'"));
	ConstructorHelpers::FObjectFinder<USoundClass>
		Effect(TEXT("/Script/Engine.SoundClass'/Game/PlatformFighterKit/Blueprints/Option/SoundOption/SC_Effect.SC_Effect'"));

	if (SoundMix.Succeeded())
	{
		MainSoundMix = SoundMix.Object;
	}
	if (BackGround.Succeeded())
	{
		SC_BackGround = BackGround.Object;
	}
	if (Effect.Succeeded())
	{
		SC_Effect = Effect.Object;
	}
}
void UOptionManager::BGSoundValue(float Value) 
{
	backgroundSound = Value;
	UGameplayStatics::SetSoundMixClassOverride(GetWorld(), MainSoundMix, SC_BackGround, backgroundSound*2);
	UGameplayStatics::PushSoundMixModifier(GetWorld(), MainSoundMix);
}
void UOptionManager::EFSoundValue(float Value)
{
	effectSound = Value;
	UGameplayStatics::SetSoundMixClassOverride(GetWorld(), MainSoundMix, SC_Effect, backgroundSound*2);
	UGameplayStatics::PushSoundMixModifier(GetWorld(), MainSoundMix);
}

void UOptionManager::ChangeScreen(EScreenMode Mode)
{
	if (CurrentMode != Mode)
	{
		CurrentMode = Mode;
		switch (CurrentMode)
		{
		case EScreenMode::FullScreen:
			GEngine->GetGameUserSettings()->SetFullscreenMode(EWindowMode::Fullscreen);
			break;
		case EScreenMode::WindowedFullScreen:
			GEngine->GetGameUserSettings()->SetFullscreenMode(EWindowMode::WindowedFullscreen);
			break;
		case EScreenMode::Window:
			GEngine->GetGameUserSettings()->SetFullscreenMode(EWindowMode::Windowed);
			break;
		default:
			break;
		}
	}
}
void UOptionManager::ChangeResolution(FIntPoint Resolution)
{
	
	if (CurrentResolution != Resolution)
	{
		CurrentResolution = Resolution;
		GEngine->GetGameUserSettings()->SetScreenResolution(CurrentResolution);
	}
}

const float UOptionManager::GetBGSound()
{
	return backgroundSound;
}
const float UOptionManager::GetEfSound()
{
	return effectSound;
}
void UOptionManager::SetBGSound(float Volume)
{
	backgroundSound = Volume;
}
void UOptionManager::SetEfSound(float Volume)
{
	effectSound = Volume;
}
