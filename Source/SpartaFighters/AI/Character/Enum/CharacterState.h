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

//�⺻ ����
// Idle �� ��� ����. ĳ���Ͱ� �ƹ��� �Է� ���� �� �ִ� ����.
// Walking �� �Ϲ����� �ȱ� ����. ���� �ӵ��� �̵��ϴ� ����.
// Crouch �� ��ũ�� ����.����� ������ ȸ���ϰų�, Ư�� ����� ����� �� ����.
// Sprint �� ���� ���� ����.������ �̵������� ������ ����� �� ���� ���� ����.
// 
//���� �� ���� ���� ����
// Jumping �� �����ϴ� ��. �������� �����ϴ� ����.
// Falling �� ���߿��� �����ϴ� ����.���� �� �߷¿� ���� �������� ����.
// 
//�ǰ� �� ���� �̻�
// Hit �� �⺻���� �ǰ� ����. ������ �¾��� �� �����.
// Launch �� �������� ���ư��� ����.���� ������ �¾� �������� ƨ������ ���.
// Ledge �� ���̳� ������ ����� �Ŵ޸� ����.
// Dead �� ü���� 0�� �Ǿ� ����� ����.
// 
//��� �� ȸ�� ����
// Shield �� ��� �ڼ�.���带 �÷� ��� ������ ���� ����.
// ShieldTumble �� ��� �ڼ����� �з����� ����.���� ������ ����ϸ� �и� �� ����.
// Dodge �� ȸ�� ����.�����⳪ ���� �̵� ���� ������.
// 
//�Ѿ��� �� ���� ����
// Tumble �� �ǰ� �� ������ �Ѿ����� ����.
// Prone �� �ٴڿ� ����� �ִ� ����.�ٿ�Ǿ��ų� Ư�� ������ �¾��� �� �߻�.
// Stun �� ���� �ð� ���� ������ �Ұ����� ����.����� ���� �����̳� �޺��� �¾��� �� �߻�.
// Dizzy �� ȥ�� ����.ĳ���Ͱ� ��û�Ÿ��� ���� �ð� ������ ���ѵ� �� ����.
// 
//Ư�� �ൿ
// Ability �� Ư�� ����� ����ϴ� ����.�ʻ�⳪ Ư�� ����� �ߵ��� �� ���.
// FreeFall �� ���� �Ҵ� ���·� �������� ��.Ư�� ����̳� �ǰ� �� ���߿��� ����� ���°� �� �� ���.
// Held �� ��뿡�� ������ �ִ� ����.��� ����� ���ϰ� ���� �� ����.
// Hold �� ��븦 ������ ����.��� ����� �����ϴ� ��.