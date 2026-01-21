// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/MonsterAnimInstance.h"
#include "Monster/MonsterBase.h"


void UMonsterAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	Owner = Cast<AMonsterBase>(GetOwningActor());
}

void UMonsterAnimInstance::NativeUpdateAnimation(float DeltaSecond)
{
	Super::NativeUpdateAnimation(DeltaSecond);

	if (Owner == nullptr)
		return;

	MoveSpeed = Owner->GetVelocity();
	IsMove = MoveSpeed.SquaredLength() > 0;

	IsDead = Owner->IsDead();
}
