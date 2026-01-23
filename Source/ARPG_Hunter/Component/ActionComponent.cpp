

#include "Component/ActionComponent.h"

#include "Subsystem/DataManager/DataManager.h"
#include "Data/WeaponTypeData.h"


UActionComponent::UActionComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	ResetAction();
}

void UActionComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UActionComponent::Init(UAnimInstance* _ownerAnimInstance)
{
	OwnerAnimInstance = _ownerAnimInstance;

	UDataManager* DataManager = GetOwner()->GetGameInstance()->GetSubsystem<UDataManager>();
	CurWeaponType = DataManager->GetWeaponTypeData(EWeaponType::SWORD);
}

void UActionComponent::ResetAction()
{
	for (uint8 i = 0; i < static_cast<uint8>(EAttackType::END); i++)
		AttackActionID[i] = 0;

	LastAttackType = EAttackType::END; 
	IsEnableNextAction = true;
}

void UActionComponent::PlayDodgeAction(bool _isMoving, TFunction<bool(float)> _predicate)
{
	if (CurWeaponType->DodgeAction.Montage == nullptr ||
		OwnerAnimInstance->Montage_IsPlaying(CurWeaponType->DodgeAction.Montage))
		return;

	if (_predicate && _predicate(CurWeaponType->DodgeAction.StaminaUsage) == false)
		return;

	OwnerAnimInstance->Montage_Play(CurWeaponType->DodgeAction.Montage);

	if (_isMoving)
		OwnerAnimInstance->Montage_JumpToSection(FName(TEXT("Fwd")), CurWeaponType->DodgeAction.Montage);
	else
		OwnerAnimInstance->Montage_JumpToSection(FName(TEXT("Bwd")), CurWeaponType->DodgeAction.Montage);
}

void UActionComponent::PlayAttackAction(EAttackType _type, TFunction<bool(float)> _predicate)
{
	if (IsValidAttackInput(_type) == false)
		return;

	FAction& Action = _type == EAttackType::NORMAL ?
		CurWeaponType->AttackAction[GetActionID(EAttackType::NORMAL)].StartAction : 
		CurWeaponType->AttackAction[GetActionID(EAttackType::NORMAL) - 1].LinkedAction[GetActionID(EAttackType::SMASH)];
	
	if (_predicate && _predicate(Action.StaminaUsage) == false)
		return;

	++AttackActionID[static_cast<uint8>(_type)];

	LastAttackType = _type;
	IsEnableNextAction = false;

	OwnerAnimInstance->Montage_Play(Action.Montage);

	SetActionResetTimer(ActionResetSecond);
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

	SetActionResetTimer(1.0f);
}

void UActionComponent::SetActionResetTimer(float _second)
{
	FTimerManager& TimerManager = GetWorld()->GetTimerManager();
	
	if (TimerManager.IsTimerActive(ActionResetTimer))
		TimerManager.ClearTimer(ActionResetTimer);

	TimerManager.SetTimer(ActionResetTimer, this, &UActionComponent::ResetAction, _second, false);
}

bool UActionComponent::IsValidAttackInput(EAttackType _type)
{
	// 다음 공격이 가능한 상태인지 확인
	// 스매시 공격 중 일반 공격으로 전환 불가
	if (IsEnableNextAction == false ||
		(LastAttackType == EAttackType::SMASH && _type == EAttackType::NORMAL) ||
		(LastAttackType == EAttackType::END && _type == EAttackType::SMASH) ||
		OwnerAnimInstance->Montage_IsPlaying(CurWeaponType->HitMontage))
		return false;

	// 마지막 콤보였는지 확인
	uint8 NormalIdx = GetActionID(EAttackType::NORMAL);
	if (_type == EAttackType::NORMAL)
		return NormalIdx < CurWeaponType->AttackAction.Num();

	if (NormalIdx >= 1 && _type == EAttackType::SMASH)
		return GetActionID(EAttackType::SMASH) < CurWeaponType->AttackAction[NormalIdx - 1].LinkedAction.Num();

	return false;
}
