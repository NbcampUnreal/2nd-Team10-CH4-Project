

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "AIBossCharacter.h"
#include "BTT_PlayBossAnim.generated.h"



UCLASS()
class SPARTAFIGHTERS_API UBTT_PlayBossAnim : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
    UBTT_PlayBossAnim();

//    UPROPERTY(EditAnywhere, Category = "Animation")
//    UAnimMontage* MontageToPlay;
//
//    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
//
//    virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
//
//private:
//    FOnMontageEnded MontageEndedDelegate;
//
//    TWeakObjectPtr<AAIBossCharacter> BossCharacter;
//
//    bool bIsMontagePlaying = false;
//
//    UFUNCTION()
//    void OnMontageEnded(UAnimMontage* Montage, bool bInterrupted);
};
