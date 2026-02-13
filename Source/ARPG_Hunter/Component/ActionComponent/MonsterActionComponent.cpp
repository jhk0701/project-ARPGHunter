// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/ActionComponent/MonsterActionComponent.h"

#include "Define/Enum.h"
#include "Data/Action.h"
#include "Data/MonsterData.h"
#include "Interface/Hitable.h"
#include "Monster/BossMonster.h"

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


void UBossActionComponent::Init(FTableRowBase* _data, TObjectPtr<UAnimInstance> _ownerAnimInstance, TObjectPtr<USkeletalMeshComponent> _firePointComp)
{
	Super::Init(_data, _ownerAnimInstance, _firePointComp);
	CurState = NORMAL;
}

bool UBossActionComponent::StartGimic(EGimicType _type, uint16 _gimicValue)
{
	if (CurState != NORMAL)
		return false;

	CurState = GIMIC;
	CurGimicType = _type;
	GimicMaxValue = _gimicValue;
	GimicValue = GimicMaxValue;

	OnGimicValueChanged.ExecuteIfBound(GimicValue, GimicMaxValue);
	OnGimicStart.ExecuteIfBound(_type);

	return true;
}

void UBossActionComponent::InterruptGimic(const FHitInfo& _hitInfo)
{
	TObjectPtr<UAnimInstance> AnimInst = GetAnimInstance();
	TObjectPtr<UAnimMontage> Montage = GetCurrentMontage();

	if (nullptr == AnimInst || nullptr == Montage)
		return;

	bool bInterrupted = false;
	switch (CurGimicType)
	{
	case EGimicType::COUNTER:
		bInterrupted = InterruptCounter(_hitInfo);
		break;
	case EGimicType::STAGGER:
		bInterrupted = InterruptStagger(_hitInfo);
		break;
	}

	if (bInterrupted)
	{
		GetAnimInstance()->Montage_JumpToSection(EnumToName(EGimicType::END), GetCurrentMontage());
		EndGimic();
	}
}

void UBossActionComponent::EndGimic()
{
	if (CurState != GIMIC)
		return;

	OnGimicEnd.ExecuteIfBound(CurGimicType);

	CurState = NORMAL;
	CurGimicType = EGimicType::END;
	GimicValue = 0;
}

bool UBossActionComponent::InterruptCounter(const FHitInfo& _hitInfo)
{
	// 맞은 범위가 정면이며, 스매시 이상 공격이었을 경우, 방해처리
	if (_hitInfo.AttackType < EAttackType::SMASH || _hitInfo.Attacker.IsValid() == false)
		return false;

	FVector AttackFwd = _hitInfo.Attacker->GetActorForwardVector();
	FVector SubjectFwd = GetOwner()->GetActorForwardVector();

	double Dot = FVector::DotProduct(AttackFwd, SubjectFwd);

	// 정면 45도 기준 : 180.0f - 45.0f
	if (Dot < 0 && FMath::RadiansToDegrees(FMath::Acos(Dot)) > 135.0f)
		GimicValue--;

	return GimicValue == 0;
}

bool UBossActionComponent::InterruptStagger(const FHitInfo& _hitInfo)
{
	if (_hitInfo.AttackType < EAttackType::SMASH || _hitInfo.Attacker.IsValid() == false)
		return false;

	if (GimicValue <= _hitInfo.StaggerDamage)
		GimicValue = 0; // 무력화 완료
	else
		GimicValue -= _hitInfo.StaggerDamage; // 무력화 진행

	OnGimicValueChanged.ExecuteIfBound(GimicValue, GimicMaxValue);

	return GimicValue == 0;
}