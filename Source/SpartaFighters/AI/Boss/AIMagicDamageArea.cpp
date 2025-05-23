


#include "AI/Boss/AIMagicDamageArea.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"

AAIMagicDamageArea::AAIMagicDamageArea()
{
    DamageCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("DamageCapsule"));
    RootComponent = DamageCapsule;
    DamageCapsule->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
    DamageCapsule->OnComponentBeginOverlap.AddDynamic(this, &AAIMagicDamageArea::OnOverlapBegin);

    MagicDamage = 0.0f;
    DamageInterval = 1.0f;
    TotalDuration = 5.0f;
}

void AAIMagicDamageArea::BeginPlay()
{
    Super::BeginPlay();

    // ������ ƽ ����
    GetWorld()->GetTimerManager().SetTimer(
        DamageTimerHandle,
        this,
        &AAIMagicDamageArea::ApplyDamageTick,
        DamageInterval,
        true
    );

    // �ڵ� �ı�
    SetLifeSpan(TotalDuration);
}

void AAIMagicDamageArea::ApplyDamageTick()
{
    for (AActor* Actor : OverlappingActors)
    {
        if (Actor && Actor != GetOwner())
        {
            UGameplayStatics::ApplyDamage(
                Actor,
                MagicDamage,
                nullptr,
                this,
                UDamageType::StaticClass()
            );
        }
    }
}

void AAIMagicDamageArea::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent,
    AActor* OtherActor,
    UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex,
    bool bFromSweep,
    const FHitResult& SweepResult)
{
    if (OtherActor && OtherActor != GetOwner())
    {
        OverlappingActors.AddUnique(OtherActor);
    }
}
