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

	/*

	GEngine->AddOnScreenDebugMessage(0, 3.0f, FColor::Red, FString::Printf(TEXT("IsMove : %d"), IsMove ? 1 : 0));
	GEngine->AddOnScreenDebugMessage(1, 3.0f, FColor::Red, FString::Printf(TEXT("(%f)"), MoveSpeed));

	*/
}
