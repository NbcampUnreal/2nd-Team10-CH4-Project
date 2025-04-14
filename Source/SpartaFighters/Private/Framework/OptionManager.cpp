
#include "Framework/OptionManager.h"
#include "Kismet/GameplayStatics.h"

UOptionManager::UOptionManager()
{
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
	UGameplayStatics::SetSoundMixClassOverride(GetWorld(), MainSoundMix, SC_BackGround, backgroundSound);
	UGameplayStatics::PushSoundMixModifier(GetWorld(), MainSoundMix);
}
void UOptionManager::EFSoundValue(float Value)
{
	effectSound = Value;
	UGameplayStatics::SetSoundMixClassOverride(GetWorld(), MainSoundMix, SC_Effect, backgroundSound);
	UGameplayStatics::PushSoundMixModifier(GetWorld(), MainSoundMix);
}

