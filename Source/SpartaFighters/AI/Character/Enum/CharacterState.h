#pragma once

#include "CoreMinimal.h"
#include "CharacterState.generated.h"

// Enum with the same name exists
UENUM(BlueprintType)
enum class ECharacterState1 : uint8
{
	Idle			UMETA(DisplayName = "Idle"),
	Walking			UMETA(DisplayName = "Walking"),
	Crouch			UMETA(DisplayName = "Crouch"),
	Sprint			UMETA(DisplayName = "Sprint"),
	Jumping			UMETA(DisplayName = "Jumping"),
	Falling			UMETA(DisplayName = "Falling"),
	Hit				UMETA(DisplayName = "Hit"),
	Launch			UMETA(DisplayName = "Launch"),
	Ledge			UMETA(DisplayName = "Ledge"),
	Dead			UMETA(DisplayName = "Dead"),
	Shield			UMETA(DisplayName = "Shield"),
	ShieldTumble	UMETA(DisplayName = "ShieldTumble"),
	Dodge			UMETA(DisplayName = "Dodge"),
	Tumble			UMETA(DisplayName = "Tumble"),
	Prone			UMETA(DisplayName = "Prone"),
	Stun			UMETA(DisplayName = "Stun"),
	Dizzy			UMETA(DisplayName = "Dizzy"),
	Ability			UMETA(DisplayName = "Ability"),
	FreeFall		UMETA(DisplayName = "FreeFall"),
	Held			UMETA(DisplayName = "Held"),
	Hold			UMETA(DisplayName = "Hold")
};

//Default state
// Idle ¡æ Standby state. The character is standing without any input.
// Walking ¡æ Normal walking state. Moving at a low speed.
// Crouch ¡æ Crouching state. Used to evade an opponent's attack or use a specific skill.
// Sprint ¡æ Running state. Moves quickly but may not be able to use attacks.
//
//States related to jumping and falling
// Jumping ¡æ Jumping. State of jumping into the air.
// Falling ¡æ State of falling from the air. State of coming down due to gravity after jumping.
//
// Hit and abnormal status
// Hit ¡æ Basic hit state. Applied when hit by an attack.
// Launch ¡æ State of flying into the air. When hit by a strong attack and bounced into the air.
// Ledge ¡æ State of hanging on a wall or railing.
// Dead ¡æ State of death with 0 health. //
//Defense and evasion
// Shield ¡æ Defensive stance. Raise your guard to block an opponent's attack.
// ShieldTumble ¡æ Being pushed back from a defensive stance. Can be pushed back if you block a strong attack.
// Dodge ¡æ Evasion moves. Includes rolling and teleportation.
//
//Falling and Stiffening
// Tumble ¡æ Rolling down after being hit.
// Prone ¡æ Lying face down on the ground. Occurs when you are downed or hit by a specific attack.
// Stun ¡æ A state where you cannot control for a certain period of time. Occurs when you are hit by a strong attack or combo from an opponent.
// Dizzy ¡æ Confusion. The character staggers and control may be restricted for a certain period of time.
//
//Special Actions
// Ability ¡æ A state where you use a specific skill. Used when activating a special move or special skill.
// FreeFall ¡æ Falling without control. Used when you become defenseless in the air after being hit by a specific skill or attack. // Held ¡æ A state where you are held by an opponent. Applies when you are being hit with a grab technique.
// Hold ¡æ A state where you are holding an opponent. Casting a grab technique.