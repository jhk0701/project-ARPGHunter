// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/Action/Player/PlayerActionComponent.h"

#include "Interface/Effectable.h"

#include "Define/Enum.h"
#include "Data/WeaponConfig.h"
#include "Data/Action.h"
#include "Data/ActionComboData.h"
#include "Data/SkillTreeData.h"
#include "Data/SkillUpgrade.h"
#include "Action/ActionInstance.h"
#include "SubObject/SubObject.h"

void UPlayerActionComponent::Init(const FPlayerActionInitParam& _param)
{
	SetAnimInstance(_param.OwnerAnimInstance);
	SetFirePointComp(_param.FirePointComp);

	// 플레이어 데이터를 기반으로 장비 모션을 적용
	CurWeapon = _param.WeaonConfig;

	const TArray<TObjectPtr<UAction>>& AttackActions = CurWeapon->AttackCombo->AttackAcionArray;
	AppliedGraph.Actions.Reserve(AttackActions.Num());
	for (const TObjectPtr<UAction>& Action : AttackActions)
	{
		// 이 액션의 기본 행동 설정
		TObjectPtr<UActionInstance> ActionInst = NewObject<UActionInstance>();
		ActionInst->SetAction(Action);
		AppliedGraph.Actions.Add(ActionInst);
	}

	// 시작점 설정
	for (const TPair<EAttackType, FConnectInfo>& Info : CurWeapon->AttackCombo->Start.Edge)
		AppliedGraph.GraphStart.Add(Info.Key, { Info.Value.Index, !Info.Value.bIsOptional });

	// 그래프 연결 
	const TArray<FActionConnection>& Connections = CurWeapon->AttackCombo->Graph;
	AppliedGraph.Graph.Reserve(Connections.Num());
	for (const FActionConnection& Connection : Connections)
	{
		// 콤보 연결 정보 구성
		TMap<EAttackType, FActionConnect> Edge;
		for (const TPair<EAttackType, FConnectInfo>& Info : Connection.Edge)
			Edge.Add(Info.Key, { Info.Value.Index, !Info.Value.bIsOptional });

		AppliedGraph.Graph.Add(Edge);
	}

	// 플레이어가 설정한 스킬 정보 반영
	FAdjustParam SkillAdjustParam;
	SkillAdjustParam.ActionArray = &AppliedGraph.Actions;
	SkillAdjustParam.Graph = &AppliedGraph.Graph;
	SkillAdjustParam.GraphStart = &AppliedGraph.GraphStart;

	TObjectPtr<USkillTreeData> SkillTree = CurWeapon->SkillTree;
	if (nullptr == SkillTree)
	{
		ResetAction();
		return;
	}

	for (const TPair<uint8, TMap<uint8, int8>>& TreePair : (*_param.SkillDevelop))
	{
		for (const TPair<uint8, int8>& NodePair : TreePair.Value)
		{
			if (NodePair.Value < 0)
				continue;

			const FSkillNode* SkillNode = SkillTree->SkillTrees[TreePair.Key].GetNode(NodePair.Key);
			if (SkillNode->UpgradeInfos.Num() <= NodePair.Value)
				continue;

			const FUpgradeInfo& UpgradeInfo = SkillNode->UpgradeInfos[NodePair.Value];
			UpgradeInfo.Upgrade->AdjustSkillNode(UpgradeInfo.TargetIndex, SkillAdjustParam);
		}
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

TWeakObjectPtr<UAction> UPlayerActionComponent::GetCurrentAction() const
{
	return AppliedGraph.Actions[CurAttackActionID]->GetAction();
}

void UPlayerActionComponent::PostProcessAttack(uint8 _opt, const TArray<FHitResult>& _inHitResults)
{
	Super::PostProcessAttack(_opt, _inHitResults);

	TObjectPtr<UActionInstance> CurAction = AppliedGraph.Actions[CurAttackActionID];
	TWeakObjectPtr<UAction> ActionData = CurAction->GetAction();

	// 자기 버프 적용
	if (CurAction->IsContainEventEffect(EActionEvent::ON_HIT))
		ActivateActionInstanceEffect(EActionEvent::ON_HIT, GetOwner(), CurAction);

	// 적에게 디버프 적용
	for (const FHitResult& Result : _inHitResults)
	{
		if (CurAction->IsContainEventEffect(EActionEvent::ON_ENEMY_HIT))
			ActivateActionInstanceEffect(EActionEvent::ON_ENEMY_HIT, Result.GetActor(), CurAction);

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


bool UPlayerActionComponent::PlayDodgeAction(bool _isMoving)
{
	TObjectPtr<UAction> DodgeAction = CurWeapon->DodgeAction;
	TWeakObjectPtr<UAnimInstance> AnimInst = GetAnimInstance();

	if (IsInProgress() || DodgeAction->Montage == nullptr ||
		AnimInst->Montage_IsPlaying(DodgeAction->Montage))
		return false;

	if (StaminaUsagePredicate.IsBound() && 
		StaminaUsagePredicate.Execute(DodgeAction->StaminaUsage) == false)
		return false;

	AnimInst->Montage_Play(DodgeAction->Montage);

	if (_isMoving)
		AnimInst->Montage_JumpToSection(FName(TEXT("Fwd")), DodgeAction->Montage);
	else
		AnimInst->Montage_JumpToSection(FName(TEXT("Bwd")), DodgeAction->Montage);

	if (DodgeAction->EventEffect.Contains(EActionEvent::ON_START))
		ActivateActionEffect(GetOwner(), DodgeAction->EventEffect[EActionEvent::ON_START].Effects);

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

bool UPlayerActionComponent::PlayAttackAction(EAttackType _type)
{
	if (IsValidAttackInput(_type) == false)
		return false;

	uint8 id = !IsInAttackCombo() ?
		AppliedGraph.GraphStart[_type].Index :
		AppliedGraph.Graph[CurAttackActionID][_type].Index;

	TWeakObjectPtr<UAction> ActionData = AppliedGraph.Actions[id]->GetAction();

	if (StaminaUsagePredicate.IsBound() && 
		StaminaUsagePredicate.Execute(AppliedGraph.Actions[id]->GetStaminaUsage()) == false)
		return false;

	CurAttackActionID = id;
	CurActionProcess = EActionProcess::START;
	CurActionInput = ActionData->InputType;
	// bIsInAttackCombo = true;
	BroadcastActionUpdated(); // SetCurrentAction(Action);


	GetAnimInstance()->Montage_Play(ActionData->Montage);
	
	ClearActionResetTimer(); // 이전 콤보에 대한 리셋 타이머 클리어

	// 액션 시작 시, 효과 발동
	if (AppliedGraph.Actions[id]->IsContainEventEffect(EActionEvent::ON_START))
		ActivateActionInstanceEffect(EActionEvent::ON_START, GetOwner(), AppliedGraph.Actions[id]);

	return true;
}

void UPlayerActionComponent::ProcessAttackProgress()
{
	if (StaminaUsagePredicate.IsBound() == false)
		return;

	// 공격 액션 지속 중, 스태미너 소모
	// 스태미너 부족 시, 바로 Complete로 진행
	if (StaminaUsagePredicate.Execute(AppliedGraph.Actions[CurAttackActionID]->GetStaminaUsage()) == false)
	{
		ProcessAttackEnd();
		return;
	}

	if (AppliedGraph.Actions[CurAttackActionID]->IsContainEventEffect(EActionEvent::ON_PROGRESS))
		ActivateActionInstanceEffect(EActionEvent::ON_PROGRESS, GetOwner(), AppliedGraph.Actions[CurAttackActionID]);
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
		AppliedGraph.GraphStart.Find(_type) :
		AppliedGraph.Graph[CurAttackActionID].Find(_type);

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

	return AppliedGraph.Actions[CurAttackActionID]->GetAction()->Montage;
}

void UPlayerActionComponent::BroadcastActionUpdated()
{
	OnActionUpdated.ExecuteIfBound(CurAttackActionID == -1, CurAttackActionID, &AppliedGraph);
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
	return AppliedGraph.Actions[CurAttackActionID]->GetAttackDamagePer(_opt);
}

uint16 UPlayerActionComponent::GetAttackActionStaggerDamage(uint8 _opt)
{	
	return AppliedGraph.Actions[CurAttackActionID]->GetStaggerDamage(_opt);
}

float UPlayerActionComponent::GetAttackActionKnockBack(uint8 _opt)
{
	return AppliedGraph.Actions[CurAttackActionID]->GetAction()->ArrOption[_opt].KnockBackStr;
}

EAttackType UPlayerActionComponent::GetAttackActionType()
{
	return  AppliedGraph.Actions[CurAttackActionID]->GetAction()->Type;
}

void UPlayerActionComponent::ActivateActionInstanceEffect(EActionEvent _type, TObjectPtr<AActor> _target, TWeakObjectPtr<UActionInstance> _actionInst)
{
	IEffectable* Effectable = Cast<IEffectable>(_target);
	if (Effectable == nullptr)
		return;

	for (const TObjectPtr<class UEffectData>& effectData : _actionInst->GetEventEffect(_type))
	{
		FApplyEffectParam Param;
		Param.Subject = GetOwner();
		Param.EffectData = effectData;
		Param.AddictiveValue = _actionInst->GetAddictiveEffectValue(effectData);

		Effectable->ApplyEffect(Param);
	}
}