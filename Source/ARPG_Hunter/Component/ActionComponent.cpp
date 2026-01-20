

#include "Component/ActionComponent.h"

#include "Subsystem/DataManager/DataManager.h"
#include "Data/WeaponTypeData.h"


UActionComponent::UActionComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
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

	LastAttackAction = nullptr;
	LastAttackType = EAttackType::END; 
	IsEnableNextAction = true;
}

bool UActionComponent::Dodge(bool _isMoving, TFunction<bool(float)> _condition)
{
	if (CurWeaponType->DodgeAction.Montage == nullptr ||
		OwnerAnimInstance->Montage_IsPlaying(CurWeaponType->DodgeAction.Montage))
		return false;

	if (_condition && _condition(CurWeaponType->DodgeAction.StaminaUsage) == false)
		return false;

	OwnerAnimInstance->Montage_Play(CurWeaponType->DodgeAction.Montage);

	if (_isMoving)
		OwnerAnimInstance->Montage_JumpToSection(FName(TEXT("Fwd")), CurWeaponType->DodgeAction.Montage);
	else
		OwnerAnimInstance->Montage_JumpToSection(FName(TEXT("Bwd")), CurWeaponType->DodgeAction.Montage);

	return true;
}

bool UActionComponent::Attack(EAttackType _type, TFunction<bool(float)> _condition)
{
	if (IsValidAttackInput(_type) == false)
		return false;

	FAction& Action = CurWeaponType->AttackAction[GetActionID(EAttackType::NORMAL)].ActionArray[GetActionID(EAttackType::SMASH)];

	if (_condition && _condition(Action.StaminaUsage) == false)
		return false;

	++AttackActionID[static_cast<uint8>(_type)];

	LastAttackType = _type;
	LastAttackAction = &Action;
	IsEnableNextAction = false;

	OwnerAnimInstance->Montage_Play(Action.Montage);

	SetActionResetTimer(ActionResetSecond);

	return true;
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
	if (IsEnableNextAction == false ||
		(LastAttackType == EAttackType::SMASH && _type == EAttackType::NORMAL))
		return false;

	uint8 NormalIdx = GetActionID(EAttackType::NORMAL);
	switch (_type)
	{
	case EAttackType::NORMAL:
		return NormalIdx < CurWeaponType->AttackAction.Num();
	case EAttackType::SMASH:
		return GetActionID(EAttackType::SMASH) < CurWeaponType->AttackAction[NormalIdx].ActionArray.Num();
	}

	return false;
}
