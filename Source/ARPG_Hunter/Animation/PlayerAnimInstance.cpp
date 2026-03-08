// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/PlayerAnimInstance.h"
#include "Player/PlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

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

	MoveSpeed.X = Owner->GetInputDirection().SquaredLength() > 0 ? 1 : 0;

	if(Owner->GetIsSprint())
		MoveSpeed *= 2;

	IsMove = MoveSpeed.X > 0;
	IsFalling = Owner->GetMovementComponent()->IsFalling();
	IsDead = Owner->IsDead();
	IsCombat = Owner->GetIsCombat();
}
