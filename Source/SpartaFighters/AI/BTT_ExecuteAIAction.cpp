


#include "BTT_ExecuteAIAction.h"
#include "AI/AICharacterController.h"
#include "Character/SFCharacter.h"
#include "Character/Components/SkillComponent.h"
#include "AI/Character/Enum/AIActionState.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"
#include "Animation/AnimInstance.h"


UBTT_ExecuteAIAction::UBTT_ExecuteAIAction()
{
    NodeName = TEXT("Play Action");
}

EBTNodeResult::Type UBTT_ExecuteAIAction::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    AAIController* AIController = OwnerComp.GetAIOwner();
    if (!AIController)
    {
        UE_LOG(LogTemp, Error, TEXT("UBTT_ExecuteAIAction::ExecuteTask - AIController is Null"));
        return EBTNodeResult::Failed;
    }

    UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
    if (!BlackboardComp)
    {
        UE_LOG(LogTemp, Error, TEXT("UBTT_ExecuteAIAction::ExecuteTask - BlackboardComp is Null"));
        return EBTNodeResult::Failed;
    }

    if (AIActionKey.SelectedKeyName == NAME_None)
    {
        UE_LOG(LogTemp, Error, TEXT("UBTT_ExecuteAIAction::ExecuteTask - AIActionKey is not set in Behavior Tree!"));
        return EBTNodeResult::Failed;
    }

    ASFCharacter* SFCharacter = Cast<ASFCharacter>(AIController->GetPawn());
    if (!SFCharacter)
    {
        UE_LOG(LogTemp, Error, TEXT("UBTT_ExecuteAIAction::ExecuteTask - AI Pawn is not an SFCharacter"));
        return EBTNodeResult::Failed;
    }

    USkillComponent* SkillComp = SFCharacter->SkillComponent;
    if (!SkillComp)
    {
        UE_LOG(LogTemp, Error, TEXT("UBTT_ExecuteAIAction::ExecuteTask - SkillComponent is Null on %s"), *GetNameSafe(SFCharacter));
        return EBTNodeResult::Failed;
    }

    UStateComponent* StateComp = SFCharacter->StateComponent;
    if (!StateComp)
    {
        UE_LOG(LogTemp, Warning, TEXT("UBTT_ExecuteAIAction::ExecuteTask - StateComponent is Null on %s (State checks disabled)"), *GetNameSafe(SFCharacter));
    }

    // 행동 중인지 먼저 확인 (선택 사항이지만 권장)
    if (StateComp && StateComp->IsInAction())
    {
        UE_LOG(LogTemp, Log, TEXT("UBTT_ExecuteAIAction::ExecuteTask - AI is already in action. Skipping new action."));
        // 이미 다른 행동 중이면 새 행동을 시작하지 않고 실패 또는 성공 처리 가능
        // 실패 처리 시 BT는 다른 대안을 찾으려 할 수 있음
        return EBTNodeResult::Failed;
    }


    // 블랙보드에서 결정된 행동 가져오기
    uint8 AIAction = BlackboardComp->GetValueAsEnum(AIActionKey.SelectedKeyName);
    // Enum 타입 이름을 실제 사용하는 것으로 변경 (EAIMeleeAction 또는 EAIActionType 등)
    EAIActionType SelectedAction = static_cast<EAIActionType>(AIAction);

    bool bActionStarted = false;

    UE_LOG(LogTemp, Log, TEXT("UBTT_ExecuteAIAction::ExecuteTask - Attempting Action: %s"), *UEnum::GetValueAsString(SelectedAction));

    // 결정된 행동에 따라 SkillComponent의 함수 호출
    switch (SelectedAction)
    {
    case EAIActionType::Attack: // 기본 공격 또는 특정 공격 스킬
        // 만약 Fireball이 기본 공격이라면 HandleInputBasicAttack()
        // SkillComp->HandleInputBasicAttack();
        // 만약 Fireball이 특정 스킬이라면 HandleInputSkillAttack()
        // 또는 구체적인 스킬 식별자가 필요하다면 SkillComponent에 새 함수 추가 고려
        SkillComp->HandleInputBasicAttack(); // 예시: SkillAttack이 Fireball을 발사한다고 가정
        bActionStarted = true;
        break;

    case EAIActionType::Guard: // 방어
        // SkillComponent에 HandleInputGuard() 함수가 구현되어 있어야 함
        if (/* SkillComp에 Guard 함수가 있다면 */ SkillComp->GetClass()->FindFunctionByName(FName("HandleInputGuard")))
        {
            // 리플렉션을 사용하거나, 직접 함수를 호출 (더 좋음)
            // SkillComp->HandleInputGuard(); // 이 함수가 있다고 가정
            UE_LOG(LogTemp, Warning, TEXT("UBTT_ExecuteAIAction: Guard action selected, assuming HandleInputGuard exists in SkillComponent."));
            // SkillComp->HandleInputGuard(); // 주석 처리 (실제 함수 존재 확인 필요)
            bActionStarted = true; // 실제 함수 호출 시 true로 변경
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("UBTT_ExecuteAIAction: Guard action selected, but HandleInputGuard function not found/implemented in SkillComponent."));
            // Guard 기능이 없으면 실패 처리
        }
        break;

    case EAIActionType::Evade: // 회피 (구르기)
        SkillComp->HandleInputDodge();
        bActionStarted = true;
        break;

    case EAIActionType::Retreat:
        // 후퇴는 보통 이동과 관련된 복합적인 행동이므로, 이 태스크에서 처리하기 부적합.
        // Behavior Tree에서 MoveTo 태스크 등을 사용하여 구현하는 것이 좋음.
        UE_LOG(LogTemp, Log, TEXT("UBTT_ExecuteAIAction: Retreat action is navigational, should be handled by MoveTo tasks. Failing this task."));
        // 실패 처리하여 BT가 다른 행동을 하도록 유도
        return EBTNodeResult::Failed;

    case EAIActionType::Chase:
        // 추격도 이동 관련 행동. MoveTo 태스크 사용.
        UE_LOG(LogTemp, Log, TEXT("UBTT_ExecuteAIAction: Chase action is navigational, should be handled by MoveTo tasks. Failing this task."));
        return EBTNodeResult::Failed;

    default:
        UE_LOG(LogTemp, Warning, TEXT("UBTT_ExecuteAIAction: Unhandled or Invalid AIActionType Enum (%d)."), AIAction);
        return EBTNodeResult::Failed; // 처리할 수 없는 액션은 실패
    }

    // 행동 시작 요청이 성공적으로 이루어졌다면 Succeeded 반환
    if (bActionStarted)
    {
        // SkillComponent의 함수는 보통 행동을 '시작'시키기만 함 (예: 몽타주 재생)
        // 실제 행동 완료는 몽타주 종료 시 또는 다른 메커니즘으로 처리됨
        // 따라서 여기서는 행동 시작 요청 성공 시 즉시 Succeeded 반환
        UE_LOG(LogTemp, Log, TEXT("UBTT_ExecuteAIAction::ExecuteTask - Action request sent successfully. Returning Succeeded."));
        return EBTNodeResult::Succeeded;
    }
    else
    {
        // 어떤 이유로든 행동 시작 요청이 실패했다면 (예: Guard 함수 없음) Failed 반환
        UE_LOG(LogTemp, Error, TEXT("UBTT_ExecuteAIAction::ExecuteTask - Failed to start the selected action."));
        return EBTNodeResult::Failed;
    }








    //UE_LOG(LogTemp, Log, TEXT("UBTT_ExecuteAIAction::ExecuteTask - Start"));

    //CachedOwnerComp = &OwnerComp;

    //AAIController* AIController = OwnerComp.GetAIOwner();
    //if (!AIController)
    //{
    //    UE_LOG(LogTemp, Error, TEXT("UBTT_ExecuteAIAction::ExecuteTask - AIController is Null"));
    //    return EBTNodeResult::Failed;
    //}

    //UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
    //if (!BlackboardComp)
    //{
    //    UE_LOG(LogTemp, Error, TEXT("UBTT_ExecuteAIAction::ExecuteTask - BlackboardComp is Null"));
    //    return EBTNodeResult::Failed;
    //}

    //ACharacter* AICharacter = Cast<ACharacter>(AIController->GetPawn());
    //if (!AICharacter)
    //{
    //    UE_LOG(LogTemp, Error, TEXT("UBTT_ExecuteAIAction::ExecuteTask - AICharacter is Null"));
    //    return EBTNodeResult::Failed;
    //}

    //UAnimInstance* AnimInstance = AICharacter->GetMesh()->GetAnimInstance();
    //if (!AnimInstance)
    //{
    //    UE_LOG(LogTemp, Error, TEXT("UBTT_ExecuteAIAction::ExecuteTask - AnimInstance is Null"));
    //    return EBTNodeResult::Failed;
    //}

    //ASFCharacter* SFCharacter = Cast<ASFCharacter>(AICharacter);
    //if (!SFCharacter)
    //{
    //    UE_LOG(LogTemp, Error, TEXT("UBTT_ExecuteAIAction::ExecuteTask - Cast to ASFCharacter failed"));
    //    return EBTNodeResult::Failed;
    //}

    //uint8 AIAction = BlackboardComp->GetValueAsEnum(AIActionKey.SelectedKeyName);

    //EAIActionType AIActionType = static_cast<EAIActionType>(AIAction);

    //switch (AIActionType)
    //{
    //case EAIActionType::Attack:
    //    SFCharacter->SpawnFireBall();
    //    break;

    //case EAIActionType::Guard:
    //    SFCharacter->SpawnFireBall();
    //    break;

    //case EAIActionType::Evade:
    //    SFCharacter->DodgePressed();
    //    break;

    //case EAIActionType::Retreat:
    //    SFCharacter->SpawnFireBall();
    //    break;
    //    
    //case EAIActionType::Chase:
    //    return EBTNodeResult::Failed;

    //default:
    //    UE_LOG(LogTemp, Warning, TEXT("Invalid AIAction Enum."));
    //    return EBTNodeResult::Failed;
    //}

    //return EBTNodeResult::Succeeded;

    //if (!MontageToPlay)
    //{
    //    UE_LOG(LogTemp, Error, TEXT("UBTT_ExecuteAIAction::ExecuteTask - MontageToPlay is Null"));
    //    return EBTNodeResult::Failed;
    //}

    // 몽타주 종료 이벤트 바인딩 (이젠 필요가 없을지도...)
    //// End Delegate 등록
    //MontageEndDelegate.BindUObject(this, &UBTT_ExecuteAIAction::OnMontageEnded);
    //AnimInstance->Montage_SetEndDelegate(MontageEndDelegate, MontageToPlay);

    //// Blending Out Delegate 등록
    //BlendingOutDelegate.BindUObject(this, &UBTT_ExecuteAIAction::OnMontageBlendingOut);
    //AnimInstance->Montage_SetBlendingOutDelegate(BlendingOutDelegate, MontageToPlay);
    //UE_LOG(LogTemp, Log, TEXT("UBTT_ExecuteAIAction::ExecuteTask - MontageEndDelegate Bind Complete"));

    //GetWorld()->GetTimerManager().SetTimer(
    //    TimerHandle,
    //    [this, AICharacter, AnimInstance]() {
    //        if (CachedOwnerComp && !AnimInstance->Montage_IsPlaying(MontageToPlay)) {
    //            UE_LOG(LogTemp, Log, TEXT("UBTT_ExecuteAIAction - Montage stopped playing, finishing task"));
    //            FinishLatentTask(*CachedOwnerComp, EBTNodeResult::Succeeded);
    //            GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
    //        }
    //    },
    //    0.1f,
    //    true
    //);

    //// 몽타주 재생
    //float PlayRate = AnimInstance->Montage_Play(MontageToPlay);
    //if (PlayRate <= 0.f)
    //{
    //    UE_LOG(LogTemp, Error, TEXT("UBTT_ExecuteAIAction::ExecuteTask - Montage dont play"));
    //    return EBTNodeResult::Failed;
    //}
    //UE_LOG(LogTemp, Log, TEXT("UBTT_ExecuteAIAction::ExecuteTask - Montage play, PlayRate: %f"), PlayRate);

    //return EBTNodeResult::InProgress;
}

void UBTT_ExecuteAIAction::OnMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
    UE_LOG(LogTemp, Log, TEXT("UBTT_ExecuteAIAction::OnMontageEnded - Montage end, bInterrupted: %d"), bInterrupted);

    if (!Montage || Montage != MontageToPlay)
    {
        UE_LOG(LogTemp, Warning, TEXT("UBTT_ExecuteAIAction::OnMontageEnded - wrong Montage end event"));
        return;
    }

    if (CachedOwnerComp)
    {
        UE_LOG(LogTemp, Log, TEXT("UBTT_ExecuteAIAction::OnMontageEnded - FinishLatentTask call"));
        FinishLatentTask(*CachedOwnerComp, bInterrupted ? EBTNodeResult::Failed : EBTNodeResult::Succeeded);
        CachedOwnerComp = nullptr; // 사용 후 초기화
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("UBTT_ExecuteAIAction::OnMontageEnded - CachedOwnerComp is null"));
    }
}

void UBTT_ExecuteAIAction::OnMontageBlendingOut(UAnimMontage* Montage, bool bInterrupted)
{
    UE_LOG(LogTemp, Log, TEXT("OnMontageBlendingOut - Montage Blending Out, bInterrupted: %d"), bInterrupted);

    if (!Montage || Montage != MontageToPlay)
    {
        return;
    }

    if (CachedOwnerComp)
    {
        FinishLatentTask(*CachedOwnerComp, EBTNodeResult::Succeeded);
        CachedOwnerComp = nullptr;
    }
}

void UBTT_ExecuteAIAction::FinishTask(UBehaviorTreeComponent* OwnerComp, EBTNodeResult::Type Result)
{
    UE_LOG(LogTemp, Log, TEXT("UBTT_ExecuteAIAction::FinishTask - Result: %d"), static_cast<uint8>(Result));
    if (OwnerComp)
    {
        FinishLatentTask(*OwnerComp, Result);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("UBTT_ExecuteAIAction::FinishTask - OwnerComp is null"));
    }
}