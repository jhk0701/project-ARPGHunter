// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/ActionComponent/PlayerActionComponent.h"

#include "Define/Enum.h"
#include "Data/WeaponConfig.h"
#include "Data/Action.h"
#include "Data/ActionComboData.h"

void UPlayerActionComponent::Init(TObjectPtr<UWeaponConfig> _data, TObjectPtr<UAnimInstance> _ownerAnimInstance, TObjectPtr<USkeletalMeshComponent> _firePointComp)
{
	Super::Init(_ownerAnimInstance, _firePointComp);

	// 플레이어 데이터를 기반으로 장비 모션을 적용
	CurWeapon = _data;
	
	const TArray<TObjectPtr<UAction>>& AttackActions = CurWeapon->AttackCombo->AttackAcionArray;
	AppliedActions.Reserve(AttackActions.Num());
	for (const TObjectPtr<UAction>& Action : AttackActions)
	{
		// 이 액션의 기본 행동 설정
		FAppliedAction NewElement;
		NewElement.Action = Action;
		AppliedActions.Add(NewElement);
		// TODO : 플레이어가 설정한 스킬 정보 반영
	}

	const TArray<FActionConnection>& Connections = CurWeapon->AttackCombo->Graph;
	AppliedGraph.Reserve(Connections.Num());
	for (const FActionConnection& Connection : Connections)
	{
		// 콤보 연결 정보 구성
		TMap<EAttackType, FActionConnect> Edge;
		for (const TPair<EAttackType, FConnectInfo>& Info : Connection.Edge)
			Edge.Add(Info.Key, { Info.Value.Index, !Info.Value.bIsOptional });

		AppliedGraph.Add(Edge);
		// TODO : 플레이어 스킬 육성에 따라 스킬 해금 여부 확인
	}

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

void UPlayerActionComponent::ResetAction()
{
	if (CurActionInput == EActionInput::HOLD)
		ClearActionProgressTimer();

	CurAttackActionID = 0;
	CurActionProcess = EActionProcess::NONE;
	CurActionInput = EActionInput::NORMAL;
	bIsInAttackCombo = false;
	SetCurrentAction(nullptr);
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
	TObjectPtr<UAnimInstance> AnimInst = GetAnimInstance();

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

	ActivateActionEffect(DodgeAction->EffectOnStart, GetOwner());
	ResetAction();

	return true;
}


void UPlayerActionComponent::PlayHitAction()
{
	if (CurWeapon->HitMontage == nullptr)
		return;

	TObjectPtr<UAnimInstance> AnimInst = GetAnimInstance();
	AnimInst->Montage_Play(CurWeapon->HitMontage);
	AnimInst->Montage_JumpToSection(FName(TEXT("Hit")), CurWeapon->HitMontage);

	// 피격 모션 실행 시, 콤보 초기화
	SetActionResetTimer(1.0f);
}

void UPlayerActionComponent::PlayDeadAction()
{
	if (CurWeapon->HitMontage == nullptr)
		return;

	TObjectPtr<UAnimInstance> AnimInst = GetAnimInstance();
	AnimInst->Montage_Play(CurWeapon->HitMontage);
	AnimInst->Montage_JumpToSection(FName(TEXT("Dead")), CurWeapon->HitMontage);
}

void UPlayerActionComponent::PlayItemUsageAction()
{
	TObjectPtr<UAnimInstance> AnimInst = GetAnimInstance();
	if (CurWeapon->ItemUsageMontage == nullptr || 
		AnimInst->Montage_IsPlaying(CurWeapon->ItemUsageMontage))
		return;

	if (bIsInAttackCombo)
		SetActionResetTimer(ActionResetSecond);

	AnimInst->Montage_Play(CurWeapon->ItemUsageMontage);
}

bool UPlayerActionComponent::PlayAttackAction(EAttackType _type, TFunction<bool(float)> _predicate)
{
	if (IsValidAttackInput(_type) == false)
		return false;

	TObjectPtr<UActionComboData> Combo = CurWeapon->AttackCombo;
	uint8 id = !bIsInAttackCombo ?
		Combo->Start.Edge[_type].Index :
		Combo->Graph[CurAttackActionID].Edge[_type].Index;

	UAction* Action = CurWeapon->AttackCombo->AttackAcionArray[id];

	if (_predicate &&
		_predicate(Action->StaminaUsage) == false)
		return false;

	CurAttackActionID = id;
	CurActionProcess = EActionProcess::START;
	CurActionInput = Action->InputType;
	bIsInAttackCombo = true;
	SetCurrentAction(Action);

	if (CurActionInput == EActionInput::HOLD)
		CurActionPredicate = _predicate;

	GetAnimInstance()->Montage_Play(Action->Montage);
	
	ClearActionResetTimer(); // 이전 콤보에 대한 리셋 타이머 클리어

	// 액션 시작 시, 효과 발동
	ActivateActionEffect(Action->EffectOnStart, GetOwner());
	return true;
}

void UPlayerActionComponent::ProcessAttackProgress()
{
	UAction* Action = CurWeapon->AttackCombo->AttackAcionArray[CurAttackActionID];

	// 공격 액션 지속 중, 스태미너 소모
	// 스태미너 부족 시, 바로 Complete로 진행
	if (CurActionPredicate(Action->StaminaUsage) == false)
	{
		ProcessAttackEnd();
		return;
	}

	ActivateActionEffect(Action->EffectOnProgress, GetOwner());
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

UAnimMontage* UPlayerActionComponent::GetDodgeMontage() const
{
	return CurWeapon->DodgeAction->Montage;
}

UAnimMontage* UPlayerActionComponent::GetHitMontage() const
{
	return CurWeapon->HitMontage;
}

// 현재 받은 공격 입력이 유효한 입력인지 확인
bool UPlayerActionComponent::IsValidAttackInput(EAttackType _type)
{
	TObjectPtr<UAnimInstance> AnimInst = GetAnimInstance();

	// 다음 공격이 가능한 상태인지 확인
	// 스매시 공격 중 일반 공격으로 전환 불가
	if (CurActionProcess < EActionProcess::COMPLETE ||
		AnimInst->Montage_IsPlaying(CurWeapon->HitMontage) ||
		AnimInst->Montage_IsPlaying(CurWeapon->ItemUsageMontage) ||
		AnimInst->Montage_IsPlaying(CurWeapon->DodgeAction->Montage))
		return false;

	if (bIsInAttackCombo == false) // 첫 공격인 경우
		return CurWeapon->AttackCombo->Start.Edge.Find(_type) != nullptr;

	// 마지막 콤보였는지 확인
	return CurWeapon->AttackCombo->Graph[CurAttackActionID].Edge.Find(_type) != nullptr;
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

void UPlayerActionComponent::SetCurrentAction(TObjectPtr<UAction> _action)
{
	Super::SetCurrentAction(_action);

	OnActionUpdated.ExecuteIfBound(_action == nullptr, CurAttackActionID, CurWeapon->AttackCombo);
}

TWeakObjectPtr<UWeaponConfig> UPlayerActionComponent::GetWeaponConfig() const
{ 
	return CurWeapon; 
}

bool UPlayerActionComponent::IsInProgress() const
{
	return CurActionProcess == EActionProcess::IN_PROGRESS;
}

uint16 UPlayerActionComponent::GetAttackActionDamagePer(uint8 _opt)
{
	return GetCurrentAction()->ArrOption[_opt].AttackDamagePer;
}

uint16 UPlayerActionComponent::GetAttackActionStaggerDamage(uint8 _opt)
{	
	return GetCurrentAction()->ArrOption[_opt].StaggerDamage;
}

float UPlayerActionComponent::GetAttackActionKnockBack(uint8 _opt)
{
	return GetCurrentAction()->ArrOption[_opt].KnockBackStr;
}

EAttackType UPlayerActionComponent::GetAttackActionType()
{
	return GetCurrentAction()->Type;
}