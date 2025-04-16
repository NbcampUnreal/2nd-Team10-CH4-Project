

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "AIBossCharacter.h"
#include "BTT_PlayBossAnimSequence.generated.h"

/**
 * 
 */
UCLASS()
class SPARTAFIGHTERS_API UBTT_PlayBossAnimSequence : public UBTTaskNode
{
	GENERATED_BODY()

public:
    UBTT_PlayBossAnimSequence();

    UPROPERTY(EditAnywhere, Category = "Animation")
    UAnimSequence* AttackSequence; // 애니메이션 시퀀스 에셋 할당

    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
    virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
    FOnMontageEnded MontageEndedDelegate;

    bool bIsPlaying = false;

    TWeakObjectPtr<AAIBossCharacter> BossCharacter;
};
