// Fill out your copyright notice in the Description page of Project Settings.

#include "Component/ActionComponent/MonsterActionComponent.h"

#include "Interface/Hitable.h"
#include "Define/Enum.h"
#include "Data/Action.h"
#include "Data/MonsterData.h"
#include "Data/MonsterConfig.h"
#include "Monster/BossMonster.h"
#include "SubObject/SubObject.h"


void UMonsterActionComponent::Init(FTableRowBase* _data, TWeakObjectPtr<UAnimInstance> _ownerAnimInstance, TWeakObjectPtr<USkeletalMeshComponent> _firePointComp)
{
	SetAnimInstance(_ownerAnimInstance);
	SetFirePointComp(_firePointComp);

	Data = static_cast<FMonsterData*>(_data);

	SetCurAttackIdx(0);
	ActionTotalWeights.Init(0.0f, static_cast<uint8>(EMonsterAttackType::END));

	for (const FMonsterAction& Action : Data->Config->AttackActions)
		ActionTotalWeights[static_cast<uint8>(Action.Type)] += Action.Weight;
}

const FMonsterAction& UMonsterActionComponent::GetCurrentMonsterAction() const
{
	return Data->Config->AttackActions[GetCurAttackIdx()];
}

TObjectPtr<UAnimMontage> UMonsterActionComponent::GetCurrentMontage() const
{
	return GetCurrentMonsterAction().Action->Montage;
}

uint16 UMonsterActionComponent::GetAttackActionDamagePer(uint8 _opt)
{
	return GetCurrentMonsterAction().Action->ArrOption[_opt].AttackDamagePer;
}

TWeakObjectPtr<class UAction> UMonsterActionComponent::GetCurrentAction() const
{
	return GetCurrentMonsterAction().Action;
}

void UMonsterActionComponent::PostProcessAttack(uint8 _opt, const TArray<FHitResult>& _inHitResults)
{
	Super::PostProcessAttack(_opt, _inHitResults);

	TWeakObjectPtr<UAction> ActionData = GetCurrentAction();

	if (ActionData->EventEffect.Contains(EActionEvent::ON_HIT))
		ActivateActionEffect(GetOwner(), ActionData->EventEffect[EActionEvent::ON_HIT].Effects);

	// 적에게 디버프 적용
	for (const FHitResult& Result : _inHitResults)
	{
		if (ActionData->EventEffect.Contains(EActionEvent::ON_ENEMY_HIT))
			ActivateActionEffect(Result.GetActor(), ActionData->EventEffect[EActionEvent::ON_ENEMY_HIT].Effects);

		// 피격 효과 출력
		if (ActionData->VFXOnHit)
		{
			SpawnHitVFX(
				ActionData->VFXOnHit,
				Result.ImpactPoint,
				ActionData->ArrOption[_opt].HitRoll,
				ActionData->ArrOption[_opt].HitSize
			);
		}
	}
}

float UMonsterActionComponent::PlayAttackAction(const FName& _opt /*= NAME_None*/)
{
	const FMonsterAction& MonsterAction = GetCurrentMonsterAction();

	TObjectPtr<UAnimMontage> AttackMontage = MonsterAction.Action->Montage;
	TWeakObjectPtr<UAnimInstance> AnimInst = GetAnimInstance();

	if (AttackMontage == nullptr ||
		AnimInst->Montage_IsPlaying(AttackMontage))
		return -1.0f;

	AnimInst->Montage_Play(AttackMontage);
	if (_opt != NAME_None)
		AnimInst->Montage_JumpToSection(_opt, AttackMontage);

	// 공격 시 자기 버프 획득
	if (MonsterAction.Action->EventEffect.Contains(EActionEvent::ON_START))
		ActivateActionEffect(GetOwner(), MonsterAction.Action->EventEffect[EActionEvent::ON_START].Effects);

	return MonsterAction.Interval;
}

void UMonsterActionComponent::PlayHitAction(EMonsterState _state)
{
	if (nullptr == Data->Config->HitMontage)
		return;

	TWeakObjectPtr<UAnimInstance> AnimInst = GetAnimInstance();
	if (false == AnimInst.IsValid())
		return;
	
	AnimInst->Montage_Play(Data->Config->HitMontage);
	if (_state == EMonsterState::DEAD)
		AnimInst->Montage_JumpToSection(EnumToName(_state), Data->Config->HitMontage);
	else
		AnimInst->Montage_JumpToSection(FName(TEXT("Hit")), Data->Config->HitMontage);
}

bool UMonsterActionComponent::PlayExtraAction(const FName& _actName)
{
	if (nullptr == Data->Config->ExtraMontage)
		return false;

	TWeakObjectPtr<UAnimInstance> AnimInst = GetAnimInstance();
	if (false == AnimInst.IsValid())
		return false;

	AnimInst->Montage_Play(Data->Config->ExtraMontage);
	AnimInst->Montage_JumpToSection(_actName);

	return true;
}

void UMonsterActionComponent::SelectAttack(EMonsterAttackType _type)
{
	SetCurAttackIdx(0);

	if (_type == EMonsterAttackType::MELEE_OR_RANGED)
	{
		if (FMath::IsNearlyZero(ActionTotalWeights[static_cast<uint8>(EMonsterAttackType::MELEE)]))
			_type = EMonsterAttackType::RANGED;
		else if (FMath::IsNearlyZero(ActionTotalWeights[static_cast<uint8>(EMonsterAttackType::RANGED)]))
			_type = EMonsterAttackType::MELEE;
		else
			_type = FMath::RandBool() ? EMonsterAttackType::MELEE : EMonsterAttackType::RANGED;
	}

	// 가중치에 따른 선별
	FMonsterData* MonsterData = GetData();
	float RandomValue = FMath::FRandRange(0.0f, ActionTotalWeights[static_cast<uint8>(_type)]);
	float Sum = 0.0f;

	for (uint8 i = 0; i < MonsterData->Config->AttackActions.Num(); ++i)
	{
		const FMonsterAction& Action = MonsterData->Config->AttackActions[i];

		if (Action.Type != _type)
			continue;

		Sum += Action.Weight;
		if (RandomValue <= Sum)
		{
			SetCurAttackIdx(i);
			break;
		}
	}
}

void UBossActionComponent::PlayHitAction(EMonsterState _state)
{
	if (nullptr == GetData()->Config->HitMontage)
		return;

	TWeakObjectPtr<UAnimInstance> AnimInst = GetAnimInstance();

	AnimInst->Montage_Play(GetData()->Config->HitMontage);
	if (_state == EMonsterState::DEAD || _state == EMonsterState::GROGGY)
		AnimInst->Montage_JumpToSection(EnumToName(_state), GetData()->Config->HitMontage);
}

bool UBossActionComponent::StartGimic(EGimicType _type, uint16 _gimicValue)
{
	CurGimicType = _type;
	GimicMaxValue = _gimicValue;
	GimicValue = GimicMaxValue;

	OnGimicValueChanged.ExecuteIfBound(GimicValue, GimicMaxValue);
	OnGimicStart.ExecuteIfBound(_type);

	return true;
}

void UBossActionComponent::InterruptGimic(const FHitInfo& _hitInfo)
{
	TWeakObjectPtr<UAnimInstance> AnimInst = GetAnimInstance();
	const FMonsterAction& MonsterAction = GetData()->Config->AttackActions[GetCurAttackIdx()];
	
	if (nullptr == AnimInst || nullptr == MonsterAction.Action->Montage)
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
		GetAnimInstance()->Montage_JumpToSection(EnumToName(EGimicType::END), MonsterAction.Action->Montage);
		EndGimic();
	}
}

void UBossActionComponent::EndGimic()
{
	OnGimicEnd.ExecuteIfBound(CurGimicType);

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