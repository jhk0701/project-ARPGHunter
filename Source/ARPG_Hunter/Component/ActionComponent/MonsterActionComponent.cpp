// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/ActionComponent/MonsterActionComponent.h"

#include "Data/Action.h"
#include "Data/MonsterData.h"

void UMonsterActionComponent::Init(FTableRowBase* _data, TObjectPtr<UAnimInstance> _ownerAnimInstance, TObjectPtr<USkeletalMeshComponent> _firePointComp)
{
	Super::Init(_data, _ownerAnimInstance, _firePointComp);

	Data = static_cast<FMonsterData*>(_data);
}

float UMonsterActionComponent::PlayAttackAction()
{
	const FMonsterAction& MonsterAction = Data->AttackActions[GetCurAttackIdx()];
	SetCurrentAction(MonsterAction.Action);

	TObjectPtr<UAnimMontage> AttackMontage = MonsterAction.Action->Montage;
	TObjectPtr<UAnimInstance> AnimInst = GetAnimInstance();

	if (AttackMontage == nullptr ||
		AnimInst->Montage_IsPlaying(CurAttackMontage))
		return -1.0f;

	AnimInst->Montage_Play(AttackMontage);
	CurAttackMontage = AttackMontage;

	return MonsterAction.Interval;
}
