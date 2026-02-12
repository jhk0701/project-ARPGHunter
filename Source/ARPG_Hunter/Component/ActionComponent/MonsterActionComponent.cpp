// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/ActionComponent/MonsterActionComponent.h"

#include "Data/Action.h"

float UMonsterActionComponent::PlayAttackAction()
{
	UAction* Action = GetCurrentAction();
	TObjectPtr<UAnimMontage> AttackMontage = Action->Montage;
	TObjectPtr<UAnimInstance> AnimInst = GetAnimInstance();

	if (AttackMontage == nullptr ||
		AnimInst->Montage_IsPlaying(CurAttackMontage))
		return -1.0f;

	AnimInst->Montage_Play(AttackMontage);
	CurAttackMontage = AttackMontage;
}
