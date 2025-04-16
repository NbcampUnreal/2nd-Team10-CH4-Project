#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EStatusType : uint8
{
    MaxHP              UMETA(DisplayName = "Max HP"),
    CurHP              UMETA(DisplayName = "Current HP"),
    MaxArmor           UMETA(DisplayName = "Max Armor"),
    CurArmor           UMETA(DisplayName = "Current Armor"),
    MaxMP              UMETA(DisplayName = "Max MP"),
    CurMP              UMETA(DisplayName = "Current MP"),
    MaxKnockbackAmount UMETA(DisplayName = "Max Knockback Amount"),
    CurKnockbackAmount UMETA(DisplayName = "Current Knockback Amount"),
    KnockbackPower     UMETA(DisplayName = "Knockback Power"),
    AttackPower        UMETA(DisplayName = "Attack Power"),
    DefensePower       UMETA(DisplayName = "Defense Power"),
    MoveSpeed          UMETA(DisplayName = "Move Speed"),
    JumpPower          UMETA(DisplayName = "Jump Power")
};
