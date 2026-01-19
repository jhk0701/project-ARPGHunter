

#include "Component/ActionComponent.h"
#include "Define/Define.h"
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

void UActionComponent::Init()
{
	UDataManager* DataManager = GetOwner()->GetGameInstance()->GetSubsystem<UDataManager>();
	CurWeaponType = DataManager->GetWeaponTypeData(EWeaponType::SWORD);
}

void UActionComponent::InitAttackAction()
{
	NormalAttack = 0;
	SmashAttack = 0;
}

const FAction& UActionComponent::GetDodgeAction()
{
	return CurWeaponType->DodgeAction;
}

const FAction& UActionComponent::GetAttackAction(bool _isNormalAttack)
{
	if (_isNormalAttack)
		return CurWeaponType->AttackAction[NormalAttack++].ActionArray[0];
	
	return CurWeaponType->AttackAction[NormalAttack].ActionArray[SmashAttack++];
}
