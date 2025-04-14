#include "Character/Mage/SFMageCharacter.h"
#include "Character/Mage/FireBall.h"
#include "Skill/TeleportEffect.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"

ASFMageCharacter::ASFMageCharacter()
{
    HatMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HatMesh"));
    HatMesh->SetupAttachment(GetMesh(), FName("head"));
    HatMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    TeleportDistance = 600.f;

}

void ASFMageCharacter::CastingFireBall()
{
    UE_LOG(LogTemp, Warning, TEXT("In Fire Ball!!"));
    FVector CastingLocation = GetMesh()->GetSocketLocation("cast_point");
    FRotator CastingRotation = GetActorRotation();

    FActorSpawnParameters SpawnParams;
    SpawnParams.Owner = this;
    SpawnParams.Instigator = GetInstigator();

    AFireBall* FireBall = GetWorld()->SpawnActor<AFireBall>(ProjectileClass, CastingLocation, CastingRotation, SpawnParams);
    if (FireBall)
    {
        FireBall->SetFireBallSpeed(1500);
        FireBall->FireInDirection(CastingRotation.Vector());
        UE_LOG(LogTemp, Warning, TEXT("Casting Fire Ball!!"));
    }
}

void ASFMageCharacter::TeleportForward()
{
    ShowTeleportDustEffect();

    GetWorldTimerManager().SetTimer(
        PrepareTeleportTimer,
        this,
        &ASFMageCharacter::DelayTeleport,
        1.0f,                      
        false                       
    );
}

void ASFMageCharacter::DelayTeleport()
{
    ExecuteTelepoprt();
    ShowTeleportCircleEffect();
    //DodgePressed();
}

void ASFMageCharacter::ShowTeleportCircleEffect()
{
    // For teleport effect spawn..
    FVector CurrentLocation = GetActorLocation();
    FVector FXLocation = CurrentLocation - FVector(0.f, 0.f, 85.f);
    FRotator FXRotation = FRotator::ZeroRotator;

    if (TeleportCircleFXClass)
    {
        GetWorld()->SpawnActor<AActor>(TeleportCircleFXClass, FXLocation, FXRotation);
    }
}

void ASFMageCharacter::ShowTeleportDustEffect()
{
    FVector CurrentLocation = GetActorLocation();
    FVector FXLocation = CurrentLocation - FVector(0.f, 0.f, 90.f);
    FRotator FXRotation = FRotator::ZeroRotator;

    if (TeleportDustFXClass)
    {
        AActor* SpawnedFX = GetWorld()->SpawnActor<AActor>(TeleportDustFXClass, FXLocation, FXRotation);
        if (SpawnedFX)
        {
            SpawnedFX->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
        }
    }
}

void ASFMageCharacter::ExecuteTelepoprt()
{
    FVector Forward = GetActorForwardVector();
    FVector CurrentLocation = GetActorLocation();
    FVector TeleportLocation = CurrentLocation + Forward * TeleportDistance;

    SetActorLocation(TeleportLocation, false, nullptr);  // TODO : If you want not sweep.. true->false change.
}

