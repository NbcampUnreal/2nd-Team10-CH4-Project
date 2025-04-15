#include "UI/UIObject/CharacterStatusWidget.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"
#include "Character/SFCharacter.h"
#include "Character/Components/StatusComponent.h"


void UCharacterStatusWidget::NativeConstruct()
{
	Super::NativeConstruct();
}


void UCharacterStatusWidget::SetPlayerName(const FString& Name)
{
	if (PlayerName)
	{
		PlayerName->SetText(FText::FromString(Name));
	}
}

void UCharacterStatusWidget::BindToCharacter(APawn* TargetCharacter)
{
	if (!TargetCharacter) return;

	// 예: 캐릭터가 ASFCharacter라면 HP 이벤트 바인딩
	if (ASFCharacter* Character = Cast<ASFCharacter>(TargetCharacter))
	{
		if (UStatusComponent* Status = Character->FindComponentByClass<UStatusComponent>())
		{
			// 델리게이트 바인딩

			Status->OnHealthChanged.AddDynamic(this, &UCharacterStatusWidget::OnCharacterHPChanged);
		}
	}
}

void UCharacterStatusWidget::UpdateHPBar(float CurrentHP, float MaxHP)
{
	if (HPBar)
	{
		float Percent = FMath::Clamp(CurrentHP / MaxHP, 0.0f, 1.0f);
		HPBar->SetPercent(Percent);
	}
}

void UCharacterStatusWidget::OnCharacterHPChanged(AActor* OwnerActor, float NewHP)
{
	const float MaxHP = 100.f;
	UpdateHPBar(NewHP, MaxHP);
}

FString UCharacterStatusWidget::GetPlayerName() const
{
	return PlayerName ? PlayerName->GetText().ToString() : TEXT("");
}