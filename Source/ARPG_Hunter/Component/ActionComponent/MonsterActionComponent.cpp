// Fill out your copyright notice in the Description page of Project Settings.

#include "Component/ActionComponent/MonsterActionComponent.h"

#include "Define/Enum.h"
#include "Data/Action.h"
#include "Data/MonsterData.h"
#include "Data/MonsterConfig.h"
#include "Interface/Hitable.h"
#include "Monster/BossMonster.h"
#include "SubObject/SubObject.h"

void UMonsterActionComponent::Init(FTableRowBase* _data, TWeakObjectPtr<UAnimInstance> _ownerAnimInstance, TWeakObjectPtr<USkeletalMeshComponent> _firePointComp)
{
	SetAnimInstance(_ownerAnimInstance);
	SetFirePointComp(_firePointComp);

	Data = static_cast<FMonsterData*>(_data);
}

void UMonsterActionComponent::ProcessAttack(uint8 _opt, ECollisionChannel _traceChannel, TFunction<void(TArray<FHitResult>&)> _onHitAction, TWeakObjectPtr<AActor> _target)
{
	Super::ProcessAttack(_opt, _traceChannel, _onHitAction, _target);

	const FMonsterAction& CurAction = GetCurrentAction();
	EAttackDetailType DetailType = CurAction.Action->ArrOption[_opt].Detail;

	if (DetailType > EAttackDetailType::MELEE_END)
	{
		// 원거리 방식 처리
		FSubObjectDeployParam DeployParam;
		DeployParam.DetailType = DetailType;
		DeployParam.SubObjectClass = CurAction.Action->SubObjectClass;
		DeployParam.SubObjectConfig = CurAction.Action->SubObjectConfig;
		DeploySubObject(DeployParam, _traceChannel, MoveTemp(_onHitAction), _target); // 기존에 받았던 람다는 Move로 이동 처리
		return;
	}

	// 근거리 방식 처리
	TArray<FHitResult> HitResults;
	FTraceParam TraceParam;
	TraceParam.DetailType = DetailType;
	TraceParam.Size = CurAction.Action->ArrOption[_opt].Size;
	TraceParam.Range = CurAction.Action->ArrOption[_opt].Range;

	bool bIsHit = Trace(TraceParam, _traceChannel, HitResults);
	if (bIsHit == false)
		return;

	// 공격 히트 시, 효과 발동
	if (_onHitAction)
		_onHitAction(HitResults);

	if (CurAction.Action->EventEffect.Contains(EActionEvent::ON_HIT))
		ActivateActionEffect(CurAction.Action->EventEffect[EActionEvent::ON_HIT].Effects, GetOwner());

	// 적에게 디버프 적용
	for (const FHitResult& Result : HitResults)
	{
		if (CurAction.Action->EventEffect.Contains(EActionEvent::ON_ENEMY_HIT))
			ActivateActionEffect(CurAction.Action->EventEffect[EActionEvent::ON_ENEMY_HIT].Effects, Result.GetActor());

		// 피격 효과 출력
		if (CurAction.Action->VFXOnHit)
		{
			SpawnHitVFX(
				CurAction.Action->VFXOnHit,
				Result.ImpactPoint,
				CurAction.Action->ArrOption[_opt].HitRoll,
				CurAction.Action->ArrOption[_opt].HitSize
			);
		}
	}
}

float UMonsterActionComponent::PlayAttackAction()
{
	const FMonsterAction& MonsterAction = GetCurrentAction();

	TObjectPtr<UAnimMontage> AttackMontage = MonsterAction.Action->Montage;
	TWeakObjectPtr<UAnimInstance> AnimInst = GetAnimInstance();

	if (AttackMontage == nullptr ||
		AnimInst->Montage_IsPlaying(MonsterAction.Action->Montage))
		return -1.0f;

	AnimInst->Montage_Play(AttackMontage);

	// 공격 시 자기 버프 획득
	if (MonsterAction.Action->EventEffect.Contains(EActionEvent::ON_START))
		ActivateActionEffect(MonsterAction.Action->EventEffect[EActionEvent::ON_START].Effects, GetOwner());

	return MonsterAction.Interval;
}

void UMonsterActionComponent::PlayHitAction(EMonsterState _state)
{
	if (nullptr == Data->Config->HitMontage)
		return;

	TWeakObjectPtr<UAnimInstance> AnimInst = GetAnimInstance();
	
	AnimInst->Montage_Play(Data->Config->HitMontage);
	if (_state == EMonsterState::DEAD)
		AnimInst->Montage_JumpToSection(EnumToName(_state), Data->Config->HitMontage);
	else
		AnimInst->Montage_JumpToSection(FName(TEXT("Hit")), Data->Config->HitMontage);
}

const FMonsterAction& UMonsterActionComponent::GetCurrentAction() const
{
	return Data->Config->AttackActions[GetCurAttackIdx()];
}

TObjectPtr<UAnimMontage> UMonsterActionComponent::GetCurrentMontage() const
{
	return GetCurrentAction().Action->Montage;
}

uint16 UMonsterActionComponent::GetAttackActionDamagePer(uint8 _opt)
{
	return GetCurrentAction().Action->ArrOption[_opt].AttackDamagePer;
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