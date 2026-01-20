

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

void UActionComponent::InitAttackAction()
{
	for (uint8 i = 0; i < static_cast<uint8>(EAttackType::END); i++)
		AttackActionID[i] = 0;

	LastAttackAction = nullptr;
	LastAttackType = EAttackType::END;
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
	if ((LastAttackAction && OwnerAnimInstance->Montage_IsPlaying(LastAttackAction->Montage)) ||
		(LastAttackType == EAttackType::SMASH && _type == EAttackType::NORMAL))
		return false;

	// AttackActionID[static_cast<uint8>(_type)];
	uint8& NormalIdx = AttackActionID[static_cast<uint8>(EAttackType::NORMAL)];
	uint8& SmashIdx = AttackActionID[static_cast<uint8>(EAttackType::SMASH)];
	FAction& Action = CurWeaponType->AttackAction[NormalIdx].ActionArray[SmashIdx];
	
	if (_type == EAttackType::NORMAL)
	{
		NormalIdx++;
		if (NormalIdx >= CurWeaponType->AttackAction.Num())
			NormalIdx = 0;
	}
	else if (_type == EAttackType::SMASH)
	{
		SmashIdx++;
		if (SmashIdx >= CurWeaponType->AttackAction[NormalIdx].ActionArray.Num())
			SmashIdx = 0;
	}

	LastAttackType = _type;
	LastAttackAction = &Action;

	if (_condition && _condition(Action.StaminaUsage) == false)
		return false;

	GEngine->AddOnScreenDebugMessage(2, 5.0f, FColor::Red, FString::Printf(TEXT("Normal Atk : %d, Smash Atk : %d"), NormalIdx, SmashIdx));

	OwnerAnimInstance->Montage_Play(Action.Montage);

	return true;
}
