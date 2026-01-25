

#include "Component/ActionComponent.h"
#include "Subsystem/DataManager/DataManager.h"

#include "Define/Enum.h"
#include "Data/WeaponTypeData.h"
#include "Data/Action.h"
#include "Data/ActionComboData.h"

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
	CurAttackActionID = 0;
	CurActionProcess = EActionProcess::NONE;
	bIsInAttackCombo = false;
}

void UActionComponent::PlayDodgeAction(bool _isMoving, TFunction<bool(float)> _predicate)
{
	TObjectPtr<UAction> DodgeAction = CurWeaponType->DodgeAction;

	if (DodgeAction->Montage == nullptr ||
		OwnerAnimInstance->Montage_IsPlaying(DodgeAction->Montage))
		return;

	if (_predicate && _predicate(DodgeAction->StaminaUsage) == false)
		return;

	OwnerAnimInstance->Montage_Play(DodgeAction->Montage);

	if (_isMoving)
		OwnerAnimInstance->Montage_JumpToSection(FName(TEXT("Fwd")), DodgeAction->Montage);
	else
		OwnerAnimInstance->Montage_JumpToSection(FName(TEXT("Bwd")), DodgeAction->Montage);
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

void UActionComponent::PlayAttackAction(EAttackType _type, TFunction<bool(float)> _predicate)
{
	if (IsValidAttackInput(_type) == false)
		return;

	uint8 id = !bIsInAttackCombo ? 
		*CurWeaponType->AttackCombo->Start.Find(_type) :
		*CurWeaponType->AttackCombo->Graph[CurAttackActionID].Edge.Find(_type);
	
	UAction* Action = CurWeaponType->AttackCombo->AttackAcionArray[id];

	if (_predicate && _predicate(Action->StaminaUsage) == false)
		return;

	CurAttackActionID = id;
	CurActionProcess = EActionProcess::START;
	bIsInAttackCombo = true;

	OwnerAnimInstance->Montage_Play(Action->Montage);

	SetActionResetTimer(ActionResetSecond);
}

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

void UActionComponent::ProcessAttackEnd()
{
	

}

void UActionComponent::SetActionResetTimer(float _second)
{
	FTimerManager& TimerManager = GetWorld()->GetTimerManager();
	
	if (TimerManager.IsTimerActive(ActionResetTimer))
		TimerManager.ClearTimer(ActionResetTimer);

	TimerManager.SetTimer(ActionResetTimer, this, &UActionComponent::ResetAction, _second, false);
}

uint16 UActionComponent::GetAttackActionDamage(uint16 _baseAttack)
{
	uint16 per = CurWeaponType->AttackCombo->AttackAcionArray[CurAttackActionID]->AttackDamagePer;
	float result = _baseAttack * (1.0f + per * 0.01f);

	return static_cast<uint16>(result);
}

uint16 UActionComponent::GetAttackActionStaggerDamage()
{
	return CurWeaponType->AttackCombo->AttackAcionArray[CurAttackActionID]->StaggerDamage;
}

float UActionComponent::GetAttackRange()
{
	return CurWeaponType->AttackCombo->AttackAcionArray[CurAttackActionID]->AttackRange;
}
