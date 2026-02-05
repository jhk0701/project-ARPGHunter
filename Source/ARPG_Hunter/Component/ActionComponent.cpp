

#include "Component/ActionComponent.h"
#include "Kismet/KismetSystemLibrary.h"

#include "Subsystem/DataManager/DataManager.h"
#include "Data/WeaponTypeData.h"
#include "Data/Action.h"
#include "Data/ActionComboData.h"

#include "Interface/Effectable.h"
#include "Data/EffectData.h"

UActionComponent::UActionComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	ResetAction();
}

void UActionComponent::Init(UAnimInstance* _ownerAnimInstance)
{
	OwnerAnimInstance = _ownerAnimInstance;

	// TODO: 플레이어 데이터를 기반으로 장비 모션을 적용
	UDataManager* DataManager = GetOwner()->GetGameInstance()->GetSubsystem<UDataManager>();
	CurWeaponType = DataManager->GetWeaponTypeData(EWeaponType::SWORD);
}

void UActionComponent::Clear()
{
	FTimerManager& TimerManager = GetWorld()->GetTimerManager();
	if (TimerManager.IsTimerActive(ActionResetTimer))
		TimerManager.ClearTimer(ActionResetTimer);
	if (TimerManager.IsTimerActive(ActionProgressTimer))
		TimerManager.ClearTimer(ActionProgressTimer);
}

void UActionComponent::ResetAction()
{
	if (CurActionInput == EActionInput::HOLD)
		ClearActionProgressTimer();

	CurAttackActionID = 0;
	CurActionProcess = EActionProcess::NONE;
	CurActionInput = EActionInput::NORMAL;
	CurActionMontage = nullptr;
	bIsInAttackCombo = false;
}

void UActionComponent::SetActionProcess(EActionProcess _eProcess)
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

		TimerManager.SetTimer(ActionProgressTimer, this, &UActionComponent::ProcessAttackProgress, ActionProgressRate, true);
	}
}

void UActionComponent::PlayDodgeAction(bool _isMoving, TFunction<bool(float)> _predicate)
{
	TObjectPtr<UAction> DodgeAction = CurWeaponType->DodgeAction;

	if (IsInProgress() || DodgeAction->Montage == nullptr ||
		OwnerAnimInstance->Montage_IsPlaying(DodgeAction->Montage))
		return;

	if (_predicate && _predicate(DodgeAction->StaminaUsage) == false)
		return;

	OwnerAnimInstance->Montage_Play(DodgeAction->Montage);

	if (_isMoving)
		OwnerAnimInstance->Montage_JumpToSection(FName(TEXT("Fwd")), DodgeAction->Montage);
	else
		OwnerAnimInstance->Montage_JumpToSection(FName(TEXT("Bwd")), DodgeAction->Montage);

	ActivateActionEffect(DodgeAction->EffectOnStart, GetOwner());
}

void UActionComponent::PlayHitAction(bool _isDead)
{
	if (CurWeaponType->HitMontage == nullptr)
		return;

	// 피격 모션 실행 시, 콤보 초기화
	OwnerAnimInstance->Montage_Play(CurWeaponType->HitMontage);

	if (_isDead)
		OwnerAnimInstance->Montage_JumpToSection(FName(TEXT("Dead")), CurWeaponType->HitMontage);
	else
		OwnerAnimInstance->Montage_JumpToSection(FName(TEXT("Hit")), CurWeaponType->HitMontage);

	SetActionResetTimer(ActionResetSecond);
}

void UActionComponent::PlayAttackAction(EAttackType _type, TFunction<bool(float)> _predicate)
{
	if (IsValidAttackInput(_type) == false)
		return;

	uint8 id = !bIsInAttackCombo ? 
		*CurWeaponType->AttackCombo->Start.Find(_type) :
		*CurWeaponType->AttackCombo->Graph[CurAttackActionID].Edge.Find(_type);
	
	UAction* Action = CurWeaponType->AttackCombo->AttackAcionArray[id];

	if (_predicate && 
		_predicate(Action->StaminaUsage) == false)
		return;

	CurAttackActionID = id;
	CurActionProcess = EActionProcess::START;
	CurActionInput = Action->InputType;
	CurActionMontage = Action->Montage;
	bIsInAttackCombo = true;

	if (CurActionInput == EActionInput::HOLD)
		CurActionPredicate = _predicate;

	OwnerAnimInstance->Montage_Play(Action->Montage);

	// 액션 시작 시, 효과 발동
	ActivateActionEffect(Action->EffectOnStart, GetOwner());
}

void UActionComponent::ProcessAttackProgress()
{
	UAction* Action = CurWeaponType->AttackCombo->AttackAcionArray[CurAttackActionID];

	// 공격 액션 지속 중, 스태미너 소모
	// 스태미너 부족 시, 바로 End 시퀀스로 이행
	if (CurActionPredicate(Action->StaminaUsage) == false)
	{
		ProcessAttackEnd();
		return;
	}

	ActivateActionEffect(Action->EffectOnProgress, GetOwner());
}

void UActionComponent::ProcessAttackEnd()
{
	if (CurActionInput < EActionInput::HOLD || 
		CurActionMontage == nullptr)
		return;

	if (CurActionProcess < EActionProcess::IN_PROGRESS)
	{
		OwnerAnimInstance->Montage_Stop(0.1f, CurActionMontage);
		ResetAction();
		return;
	}
	else if (CurActionProcess == EActionProcess::COMPLETE)
		return;
	
	CurActionProcess = EActionProcess::COMPLETE; // 종료 상태로 변경

	// 누르는 입력이 종료됨
	// 현재 재생중인 몽타주를 강제로 Complete 섹션으로 전환
	OwnerAnimInstance->Montage_JumpToSection(EnumToName(EActionProcess::COMPLETE), CurActionMontage);

	ClearActionProgressTimer();
}

// 현재 받은 공격 입력이 유효한 입력인지 확인
bool UActionComponent::IsValidAttackInput(EAttackType _type)
{
	// 다음 공격이 가능한 상태인지 확인
	// 스매시 공격 중 일반 공격으로 전환 불가
	if (CurActionProcess < EActionProcess::COMPLETE ||
		OwnerAnimInstance->Montage_IsPlaying(CurWeaponType->HitMontage))
		return false;

	if (bIsInAttackCombo == false) // 첫 공격인 경우
		return CurWeaponType->AttackCombo->Start.Find(_type) != nullptr;

	// 마지막 콤보였는지 확인
	return CurWeaponType->AttackCombo->Graph[CurAttackActionID].Edge.Find(_type) != nullptr;
}

void UActionComponent::SetActionResetTimer(float _second)
{
	FTimerManager& TimerManager = GetWorld()->GetTimerManager();
	
	if (TimerManager.IsTimerActive(ActionResetTimer))
		TimerManager.ClearTimer(ActionResetTimer);

	TimerManager.SetTimer(ActionResetTimer, this, &UActionComponent::ResetAction, _second, false);
}

uint16 UActionComponent::GetAttackActionDamagePer()
{
	return CurWeaponType->AttackCombo->AttackAcionArray[CurAttackActionID]->AttackDamagePer;
}
uint16 UActionComponent::GetAttackActionStaggerDamage()
{
	return CurWeaponType->AttackCombo->AttackAcionArray[CurAttackActionID]->StaggerDamage;
}
float UActionComponent::GetAttackActionKnockBack(uint8 _opt)
{
	return CurWeaponType->AttackCombo->AttackAcionArray[CurAttackActionID]->ArrOption[_opt].KnockBackStr;
}

bool UActionComponent::TraceAttack(uint8 _opt, TArray<FHitResult>& _outHitResult)
{
	UAction* CurAction = CurWeaponType->AttackCombo->AttackAcionArray[CurAttackActionID];
	if (CurAction == nullptr || 
		CurAction->ArrOption.Num() <= _opt)
		return false;

	const FActionOption& Option = CurAction->ArrOption[_opt];
	bool IsHit = false;

	FVector ActorLoc = GetOwner()->GetActorLocation();
	FVector ActorFwd = GetOwner()->GetActorForwardVector();

	switch (Option.Direction)
	{
	case EAttackDirection::FRONT:
		IsHit = UKismetSystemLibrary::BoxTraceMulti(
			GetWorld(),
			ActorLoc + ActorFwd * 100.0f,
			ActorLoc + ActorFwd * 100.0f,
			FVector(Option.Range, 100, 100),
			ActorFwd.Rotation(),
			UEngineTypes::ConvertToTraceType(ECC_GameTraceChannel4),
			false, { GetOwner() },
			EDrawDebugTrace::None,
			_outHitResult,
			true
		);
		break;
	case EAttackDirection::FRONT_WIDE:
		IsHit = UKismetSystemLibrary::BoxTraceMulti(
			GetWorld(),
			ActorLoc + ActorFwd * 100.0f,
			ActorLoc + ActorFwd * 100.0f,
			FVector(100, Option.Range, 100),
			ActorFwd.Rotation(),
			UEngineTypes::ConvertToTraceType(ECC_GameTraceChannel4),
			false, { GetOwner() },
			EDrawDebugTrace::None,
			_outHitResult,
			true
		);
		break;
	case EAttackDirection::AROUND:
		IsHit = UKismetSystemLibrary::SphereTraceMulti(
			GetWorld(),
			ActorLoc, ActorLoc,
			Option.Range,
			UEngineTypes::ConvertToTraceType(ECC_GameTraceChannel4),
			false, { GetOwner() },
			EDrawDebugTrace::None,
			_outHitResult,
			true
		);
		break;
	}

	// 공격 히트 시, 효과 발동
	// 자기 버프
	ActivateActionEffect(CurAction->EffectOnHit, GetOwner());

	return IsHit;
}

void UActionComponent::ActivateActionEffect(const TArray<TObjectPtr<class UEffectData>>& _effectArray, TObjectPtr<AActor> _target)
{
	IEffectable* Effectable = Cast<IEffectable>(_target);

	if (Effectable == nullptr)
		return;

	for (const TObjectPtr<class UEffectData>& effectData : _effectArray)
		Effectable->ApplyEffect(effectData->Effect, &effectData->Param);
}

void UActionComponent::ClearActionProgressTimer()
{
	FTimerManager& TimerManager = GetWorld()->GetTimerManager();
	if (TimerManager.IsTimerActive(ActionProgressTimer))
		TimerManager.ClearTimer(ActionProgressTimer);
}
