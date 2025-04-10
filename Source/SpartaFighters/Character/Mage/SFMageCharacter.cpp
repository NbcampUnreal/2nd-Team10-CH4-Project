#include "Character/Mage/SFMageCharacter.h"
#include "Character/Mage/FireBall.h"

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
    FVector Forward = GetActorForwardVector();
    FVector CurrentLocation = GetActorLocation();
    FVector TeleportLocation = CurrentLocation + Forward * TeleportDistance;

    SetActorLocation(TeleportLocation, true, nullptr);  // TODO : If you want not sweep.. true->false change.
}