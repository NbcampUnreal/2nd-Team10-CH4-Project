#include "UI/UIElements/CombatHUD.h"

#include "Framework/SFGameInstance.h"
#include "GameFramework/GameState.h"
#include "GameFramework/PlayerState.h"
#include "GameFramework/PlayerController.h"

#include "Kismet/GameplayStatics.h"
#include "Components/HorizontalBox.h"
#include "Components/HorizontalBoxSlot.h"

#include "UI/UIObject/CharacterStatusWidget.h"

void UCombatHUD::NativeConstruct()
{
	Super::NativeConstruct();

	GetWorld()->GetTimerManager().SetTimer(
		InitCreateWidgetTimer,
		this,
		&UCombatHUD::CreateCharacterStatusWidget,
		1.0f,
		false
	);

	//CreateCharacterStatusWidget();
}

void UCombatHUD::CreateCharacterStatusWidget()
{
	if (!StatusContainer)
	{
		return;
	}

	StatusContainer->ClearChildren();


	const UWorld* World = GetWorld();
	if (World == nullptr)
	{
		ensure(false);
		return;
	}
	const AGameStateBase* GS = World->GetGameState();
	if (GS == nullptr)
	{
		ensure(false);
		return;
	}
	const APlayerController* PC = UGameplayStatics::GetPlayerController(World, 0);
	if (PC == nullptr)
	{
		ensure(false);
		return;
	}
	const APlayerState* PS = PC->PlayerState;
	if (PS == nullptr)
	{
		ensure(false);
		return;
	}
	const TArray<APlayerState*>& Players = GS->PlayerArray;
	if (Players.Num() == 0)
	{
		return;
	}
	const FString LocalID = PS->GetPlayerName();


	TArray<AActor*> PlayerControllers;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerController::StaticClass(), PlayerControllers);

	for (APlayerState* CurrentPlayerState : Players)
	{
		//APlayerController* PC = Cast<APlayerController>(PlayerControllers[i]);
		//if (!PC || !PC->GetPawn()) continue;

		UCharacterStatusWidget* NewWidget = CreateWidget<UCharacterStatusWidget>(GetWorld(), CharacterStatusWidget);
		if (NewWidget)
		{
			StatusContainer->AddChildToHorizontalBox(NewWidget);
			NewWidget->SetPadding(FMargin(10.f, 0.f));

			//if (PS)
			//{
			//	NewWidget->SetPlayerName(PS->GetPlayerName());
			//}
			//auto pawn = PC->GetPawn();
			//NewWidget->BindToCharacter(pawn);

			NewWidget->SetPlayerName(CurrentPlayerState->GetPlayerName());

			APawn* FoundPawn = nullptr;
			for (AActor* Actor : PlayerControllers)
			{
				APlayerController* Controller = Cast<APlayerController>(Actor);
				if (Controller && Controller->PlayerState == CurrentPlayerState)
				{
					FoundPawn = Controller->GetPawn();
					break;
				}
			}

			if (FoundPawn)
			{
				NewWidget->BindToCharacter(FoundPawn);
			}

			// HorizontalBox에 추가
			//UHorizontalBoxSlot* NewSlot = StatusContainer->AddChildToHorizontalBox(NewWidget);
			//if (NewSlot)
			//{
			//	NewSlot->SetPadding(FMargin(10.f));
			//	NewSlot->SetHorizontalAlignment(HAlign_Left);
			//	NewSlot->SetVerticalAlignment(VAlign_Center);
			//}
		}
	}
}

void UCombatHUD::RebindCharacterStatus(APawn* NewPawn)
{
	for (UWidget* Widget : StatusContainer->GetAllChildren())
	{
		if (UCharacterStatusWidget* StatusWidget = Cast<UCharacterStatusWidget>(Widget))
		{
			// 플레이어 이름이 일치하는 위젯 찾기
			if (StatusWidget->GetPlayerName() == NewPawn->GetPlayerState()->GetPlayerName())
			{
				StatusWidget->BindToCharacter(NewPawn);
				break;
			}
		}
	}
}