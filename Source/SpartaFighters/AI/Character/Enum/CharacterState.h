#pragma once

#include "CoreMinimal.h"
#include "CharacterState.generated.h"


UENUM(BlueprintType)
enum class ECharacterState : uint8
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

//기본 상태
// Idle → 대기 상태. 캐릭터가 아무런 입력 없이 서 있는 상태.
// Walking → 일반적인 걷기 상태. 낮은 속도로 이동하는 상태.
// Crouch → 웅크린 상태.상대의 공격을 회피하거나, 특정 기술을 사용할 때 사용됨.
// Sprint → 전력 질주 상태.빠르게 이동하지만 공격을 사용할 수 없을 수도 있음.
// 
//점프 및 낙하 관련 상태
// Jumping → 점프하는 중. 공중으로 도약하는 상태.
// Falling → 공중에서 낙하하는 상태.점프 후 중력에 의해 내려오는 상태.
// 
//피격 및 상태 이상
// Hit → 기본적인 피격 상태. 공격을 맞았을 때 적용됨.
// Launch → 공중으로 날아가는 상태.강한 공격을 맞아 공중으로 튕겨지는 경우.
// Ledge → 벽이나 난간을 붙잡고 매달린 상태.
// Dead → 체력이 0이 되어 사망한 상태.
// 
//방어 및 회피 관련
// Shield → 방어 자세.가드를 올려 상대 공격을 막는 상태.
// ShieldTumble → 방어 자세에서 밀려나는 상태.강한 공격을 방어하면 밀릴 수 있음.
// Dodge → 회피 동작.구르기나 순간 이동 등을 포함함.
// 
//넘어짐 및 경직 상태
// Tumble → 피격 후 구르며 넘어지는 상태.
// Prone → 바닥에 엎드려 있는 상태.다운되었거나 특정 공격을 맞았을 때 발생.
// Stun → 일정 시간 동안 조작이 불가능한 상태.상대의 강한 공격이나 콤보를 맞았을 때 발생.
// Dizzy → 혼란 상태.캐릭터가 휘청거리며 일정 시간 조작이 제한될 수 있음.
// 
//특수 행동
// Ability → 특정 기술을 사용하는 상태.필살기나 특수 기술을 발동할 때 사용.
// FreeFall → 통제 불능 상태로 떨어지는 중.특정 기술이나 피격 후 공중에서 무방비 상태가 될 때 사용.
// Held → 상대에게 붙잡혀 있는 상태.잡기 기술을 당하고 있을 때 적용.
// Hold → 상대를 붙잡은 상태.잡기 기술을 시전하는 중.