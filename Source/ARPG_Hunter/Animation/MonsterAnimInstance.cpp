// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/MonsterAnimInstance.h"
#include "Monster/MonsterBase.h"

void UMonsterAnimInstance::NativeUpdateAnimation(float DeltaSecond)
{
	Super::NativeUpdateAnimation(DeltaSecond);

	AMonsterBase* Owner = Cast<AMonsterBase>(TryGetPawnOwner());
	if (Owner == nullptr)
		return;

	MoveSpeed = Owner->GetVelocity().Size();
	IsMove = MoveSpeed > 0.0f;
	IsDead = Owner->IsDead();

}
