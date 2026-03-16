// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/ActionComponent/PlayerActionComponent.h"

#include "Define/Enum.h"
#include "Data/WeaponConfig.h"
#include "Action/ActionInstance.h"
#include "Data/Action.h"
#include "Data/ActionComboData.h"
#include "SubObject/SubObject.h"

void UPlayerActionComponent::Init(TObjectPtr<UWeaponConfig> _data, TWeakObjectPtr<UAnimInstance> _ownerAnimInstance, TWeakObjectPtr<USkeletalMeshComponent> _firePointComp)
{
	SetAnimInstance(_ownerAnimInstance);
	SetFirePointComp(_firePointComp);

	// 플레이어 데이터를 기반으로 장비 모션을 적용
	CurWeapon = _data;
	
	const TArray<TObjectPtr<UAction>>& AttackActions = CurWeapon->AttackCombo->AttackAcionArray;
	AppliedActions.Reserve(AttackActions.Num());
	for (const TObjectPtr<UAction>& Action : AttackActions)
	{
		// 이 액션의 기본 행동 설정
		TObjectPtr<UActionInstance> ActionInst = NewObject<UActionInstance>();
		ActionInst->SetAction(Action);
		AppliedActions.Add(ActionInst);
	}

	// 시작점 설정
	for (const TPair<EAttackType, FConnectInfo>& Info : CurWeapon->AttackCombo->Start.Edge)
		AppliedGraphStart.Add(Info.Key, { Info.Value.Index, true /*!Info.Value.bIsOptional*/ });

	// 그래프 연결 
	const TArray<FActionConnection>& Connections = CurWeapon->AttackCombo->Graph;
	AppliedGraph.Reserve(Connections.Num());
	for (const FActionConnection& Connection : Connections)
	{
		// 콤보 연결 정보 구성
		TMap<EAttackType, FActionConnect> Edge;
		for (const TPair<EAttackType, FConnectInfo>& Info : Connection.Edge)
			Edge.Add(Info.Key, { Info.Value.Index, true /*!Info.Value.bIsOptional*/ });

		AppliedGraph.Add(Edge);
	}

	// TODO : 플레이어가 설정한 스킬 정보 반영
	// TODO : 플레이어 스킬 육성에 따라 스킬 해금 여부 확인

	ResetAction();
}

void UPlayerActionComponent::Clear()
{
	Super::Clear();

	FTimerManager& TimerManager = GetWorld()->GetTimerManager();
	if (TimerManager.IsTimerActive(ActionResetTimer))
		TimerManager.ClearTimer(ActionResetTimer);
	if (TimerManager.IsTimerActive(ActionProgressTimer))
		TimerManager.ClearTimer(ActionProgressTimer);
}

void UPlayerActionComponent::ProcessAttack(uint8 _opt, ECollisionChannel _traceChannel, TFunction<void(TArray<FHitResult>&)> _onHitAction, TWeakObjectPtr<AActor> _target)
{
	Super::ProcessAttack(_opt, _traceChannel, _onHitAction, _target);

	TObjectPtr<UActionInstance> CurAction = AppliedActions[CurAttackActionID];
	TWeakObjectPtr<UAction> ActionData = CurAction->GetAction();
	EAttackDetailType DetailType = ActionData->ArrOption[_opt].Detail;

	if (DetailType > EAttackDetailType::MELEE_END)
	{
		// 원거리 방식 처리
		FSubObjectDeployParam DeployParam;
		DeployParam.DetailType = DetailType;
		DeployParam.SubObjectClass = ActionData->SubObjectClass;
		DeployParam.SubObjectConfig = ActionData->SubObjectConfig;
		DeploySubObject(DeployParam, _traceChannel, MoveTemp(_onHitAction), _target); // 기존에 받았던 람다는 Move로 이동 처리
		return;
	}

	// 근거리 방식 처리
	TArray<FHitResult> HitResults;
	FTraceParam TraceParam;
	TraceParam.DetailType = DetailType;
	TraceParam.Size = ActionData->ArrOption[_opt].Size;
	TraceParam.Range = ActionData->ArrOption[_opt].Range;

	bool bIsHit = Trace(TraceParam, _traceChannel, HitResults);
	if (bIsHit == false)
		return;

	// 공격 히트 시, 효과 발동
	if (_onHitAction)
		_onHitAction(HitResults);

	// 자기 버프 적용
	if (CurAction->IsContainEventEffect(EActionEvent::ON_HIT))
		ActivateActionEffect(CurAction->GetEventEffect(EActionEvent::ON_HIT), GetOwner());

	// 적에게 디버프 적용
	for (const FHitResult& Result : HitResults)
	{
		if (CurAction->IsContainEventEffect(EActionEvent::ON_ENEMY_HIT))
			ActivateActionEffect(CurAction->GetEventEffect(EActionEvent::ON_ENEMY_HIT), Result.GetActor());

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

void UPlayerActionComponent::ResetAction()
{
	if (CurActionInput == EActionInput::HOLD)
		ClearActionProgressTimer();

	CurAttackActionID = -1;
	CurActionProcess = EActionProcess::NONE;
	CurActionInput = EActionInput::NORMAL;
	// bIsInAttackCombo = false;
	BroadcastActionUpdated(); // SetCurrentAction(nullptr);
}

void UPlayerActionComponent::SetActionProcess(EActionProcess _eProcess)
{
	CurActionProcess = _eProcess;

	if (CurActionProcess == EActionProcess::COMPLETE)
		SetActionResetTimer(ActionResetSecond);
	else if (CurActionProcess == EActionProcess::IN_PROGRESS &&
		CurActionInput == EActionInput::HOLD)
	{
		FTimerManager& TimerManager = GetWorld()->GetTimerManager();
		if (TimerManager.IsTimerActive(ActionProgressTimer))
			TimerManager.ClearTimer(ActionProgressTimer);

		TimerManager.SetTimer(ActionProgressTimer, this, &UPlayerActionComponent::ProcessAttackProgress, ActionProgressRate, true);
	}
}


bool UPlayerActionComponent::PlayDodgeAction(bool _isMoving, TFunction<bool(float)> _predicate)
{
	TObjectPtr<UAction> DodgeAction = CurWeapon->DodgeAction;
	TWeakObjectPtr<UAnimInstance> AnimInst = GetAnimInstance();

	if (IsInProgress() || DodgeAction->Montage == nullptr ||
		AnimInst->Montage_IsPlaying(DodgeAction->Montage))
		return false;

	if (_predicate && _predicate(DodgeAction->StaminaUsage) == false)
		return false;

	AnimInst->Montage_Play(DodgeAction->Montage);

	if (_isMoving)
		AnimInst->Montage_JumpToSection(FName(TEXT("Fwd")), DodgeAction->Montage);
	else
		AnimInst->Montage_JumpToSection(FName(TEXT("Bwd")), DodgeAction->Montage);

	if (DodgeAction->EventEffect.Contains(EActionEvent::ON_START))
		ActivateActionEffect(DodgeAction->EventEffect[EActionEvent::ON_START].Effects, GetOwner());

	ResetAction();

	return true;
}


void UPlayerActionComponent::PlayHitAction()
{
	if (CurWeapon->HitMontage == nullptr)
		return;

	TWeakObjectPtr<UAnimInstance> AnimInst = GetAnimInstance();
	if (AnimInst.IsValid() == false) return;

	AnimInst->Montage_Play(CurWeapon->HitMontage);
	AnimInst->Montage_JumpToSection(FName(TEXT("Hit")), CurWeapon->HitMontage);

	// 피격 모션 실행 시, 콤보 초기화
	SetActionResetTimer(1.0f);
}

void UPlayerActionComponent::PlayDeadAction()
{
	if (CurWeapon->HitMontage == nullptr)
		return;

	TWeakObjectPtr<UAnimInstance> AnimInst = GetAnimInstance();
	if (AnimInst.IsValid() == false) return;

	AnimInst->Montage_Play(CurWeapon->HitMontage);
	AnimInst->Montage_JumpToSection(FName(TEXT("Dead")), CurWeapon->HitMontage);
}

void UPlayerActionComponent::PlayItemUsageAction()
{
	TWeakObjectPtr<UAnimInstance> AnimInst = GetAnimInstance();
	if (AnimInst.IsValid() == false) return;

	if (CurWeapon->ItemUsageMontage == nullptr || 
		AnimInst->Montage_IsPlaying(CurWeapon->ItemUsageMontage))
		return;

	if (IsInAttackCombo())
		SetActionResetTimer(ActionResetSecond);

	AnimInst->Montage_Play(CurWeapon->ItemUsageMontage);
}

bool UPlayerActionComponent::PlayAttackAction(EAttackType _type, TFunction<bool(float)> _predicate)
{
	if (IsValidAttackInput(_type) == false)
		return false;

	uint8 id = !IsInAttackCombo() ?
		AppliedGraphStart[_type].Index :
		AppliedGraph[CurAttackActionID][_type].Index;

	TWeakObjectPtr<UAction> ActionData = AppliedActions[id]->GetAction();

	if (_predicate && _predicate(AppliedActions[id]->GetStaminaUsage()) == false)
		return false;

	CurAttackActionID = id;
	CurActionProcess = EActionProcess::START;
	CurActionInput = ActionData->InputType;
	// bIsInAttackCombo = true;
	BroadcastActionUpdated(); // SetCurrentAction(Action);

	if (CurActionInput == EActionInput::HOLD)
		CurActionPredicate = _predicate;

	GetAnimInstance()->Montage_Play(ActionData->Montage);
	
	ClearActionResetTimer(); // 이전 콤보에 대한 리셋 타이머 클리어

	// 액션 시작 시, 효과 발동
	if (AppliedActions[id]->IsContainEventEffect(EActionEvent::ON_START))
		ActivateActionEffect(AppliedActions[id]->GetEventEffect(EActionEvent::ON_START), GetOwner());

	return true;
}

void UPlayerActionComponent::ProcessAttackProgress()
{
	// 공격 액션 지속 중, 스태미너 소모
	// 스태미너 부족 시, 바로 Complete로 진행
	if (CurActionPredicate(AppliedActions[CurAttackActionID]->GetStaminaUsage()) == false)
	{
		ProcessAttackEnd();
		return;
	}

	if (AppliedActions[CurAttackActionID]->IsContainEventEffect(EActionEvent::ON_PROGRESS))
		ActivateActionEffect(AppliedActions[CurAttackActionID]->GetEventEffect(EActionEvent::ON_PROGRESS), GetOwner());
}

void UPlayerActionComponent::ProcessAttackEnd()
{
	TObjectPtr<UAnimMontage> CurMontage = GetCurrentMontage();
	if (CurActionInput < EActionInput::HOLD || nullptr == CurMontage)
		return;

	if (CurActionProcess < EActionProcess::IN_PROGRESS)
	{
		GetAnimInstance()->Montage_Stop(0.1f, CurMontage);
		ResetAction();
		return;
	}
	else if (CurActionProcess == EActionProcess::COMPLETE)
		return;

	CurActionProcess = EActionProcess::COMPLETE; // 종료 상태로 변경

	// 누르는 입력이 종료됨
	// 현재 재생중인 몽타주를 강제로 Complete 섹션으로 전환
	GetAnimInstance()->Montage_JumpToSection(EnumToName(EActionProcess::COMPLETE), CurMontage);

	ClearActionProgressTimer();
}


// 현재 공격 입력이 유효한지 확인
bool UPlayerActionComponent::IsValidAttackInput(EAttackType _type)
{
	TWeakObjectPtr<UAnimInstance> AnimInst = GetAnimInstance();
	if (AnimInst.IsValid() == false) 
		return false;

	// 다음 공격이 가능한 상태인지 확인
	// 스매시 공격 중 일반 공격으로 전환 불가
	if (CurActionProcess < EActionProcess::COMPLETE ||
		AnimInst->Montage_IsPlaying(CurWeapon->HitMontage) ||
		AnimInst->Montage_IsPlaying(CurWeapon->ItemUsageMontage) ||
		AnimInst->Montage_IsPlaying(CurWeapon->DodgeAction->Montage))
		return false;
	
	FActionConnect* ActionConnect = IsInAttackCombo() == false ? 
		AppliedGraphStart.Find(_type) :
		AppliedGraph[CurAttackActionID].Find(_type);

	// 할당된 공격이 있는지, 해당 공격이 해금되었는지 확인
	return nullptr != ActionConnect && ActionConnect->bIsUnlocked;
}

void UPlayerActionComponent::SetActionResetTimer(float _second)
{
	FTimerManager& TimerManager = GetWorld()->GetTimerManager();

	if (TimerManager.IsTimerActive(ActionResetTimer))
		TimerManager.ClearTimer(ActionResetTimer);

	TimerManager.SetTimer(ActionResetTimer, this, &UPlayerActionComponent::ResetAction, _second, false);
}

void UPlayerActionComponent::ClearActionResetTimer()
{
	FTimerManager& TimerManager = GetWorld()->GetTimerManager();

	if (TimerManager.IsTimerActive(ActionResetTimer))
		TimerManager.ClearTimer(ActionResetTimer);
}

void UPlayerActionComponent::ClearActionProgressTimer()
{
	FTimerManager& TimerManager = GetWorld()->GetTimerManager();
	if (TimerManager.IsTimerActive(ActionProgressTimer))
		TimerManager.ClearTimer(ActionProgressTimer);
}

TObjectPtr<UAnimMontage> UPlayerActionComponent::GetCurrentMontage()
{
	if (CurAttackActionID < 0)
		return nullptr;

	return AppliedActions[CurAttackActionID]->GetAction()->Montage;
}

void UPlayerActionComponent::BroadcastActionUpdated()
{
	OnActionUpdated.ExecuteIfBound(CurAttackActionID == -1, CurAttackActionID, CurWeapon->AttackCombo);
}

TObjectPtr<UAnimMontage> UPlayerActionComponent::GetDodgeMontage() const
{
	return CurWeapon->DodgeAction->Montage;
}

TObjectPtr<UAnimMontage> UPlayerActionComponent::GetHitMontage() const
{
	return CurWeapon->HitMontage;
}

bool UPlayerActionComponent::IsInProgress() const
{
	return CurActionProcess == EActionProcess::IN_PROGRESS;
}

uint16 UPlayerActionComponent::GetAttackActionDamagePer(uint8 _opt)
{
	return AppliedActions[CurAttackActionID]->GetAttackDamagePer(_opt);
}

uint16 UPlayerActionComponent::GetAttackActionStaggerDamage(uint8 _opt)
{	
	return AppliedActions[CurAttackActionID]->GetStaggerDamage(_opt);
}

float UPlayerActionComponent::GetAttackActionKnockBack(uint8 _opt)
{
	return AppliedActions[CurAttackActionID]->GetAction()->ArrOption[_opt].KnockBackStr;
}

EAttackType UPlayerActionComponent::GetAttackActionType()
{
	return  AppliedActions[CurAttackActionID]->GetAction()->Type;
}