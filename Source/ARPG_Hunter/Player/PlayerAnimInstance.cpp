// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/PlayerAnimInstance.h"
#include "Player/PlayerCharacter.h"

void UPlayerAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	Owner = Cast<APlayerCharacter>(TryGetPawnOwner());
}

void UPlayerAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	if (nullptr == Owner)
		return;

	MoveSpeed = Owner->GetInputDirection() * (Owner->GetIsSprint() ? 2 : 1);
	IsMove = Owner->GetVelocity().SizeSquared() > 0.0;
}
