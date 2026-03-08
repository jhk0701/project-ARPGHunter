// Fill out your copyright notice in the Description page of Project Settings.


#include "Effect/TermEffect/TermEffect.h"
#include "Data/EffectData.h"

bool UTermEffect::Activate()
{
	if (!IsValid())
		return false;

	// 효과의 유효기간 타이머 설정
	// 타이머 경과 이후, Deactivate 호출
	if (GetTarget()->RegisterEffect(this) == false)
		return false;

	return Super::Activate();
}

void UTermEffect::Deactivate(){}

bool UAddAttack::Activate()
{
	if (!Super::Activate()) 
		return false;

	GetTarget()->AddStat(ECharacterStatType::ATTACK, GetParam()->Value);
	return true;
}

void UAddAttack::Deactivate()
{
	if (!IsValid()) return;

	GetTarget()->SubStat(ECharacterStatType::ATTACK, GetParam()->Value * GetStack());
}

bool UAddDefense::Activate()
{
	if (!Super::Activate())
		return false;

	GetTarget()->AddStat(ECharacterStatType::DEFENSE, GetParam()->Value);
	return true;
}

void UAddDefense::Deactivate()
{
	if (!IsValid()) return;

	GetTarget()->SubStat(ECharacterStatType::DEFENSE, GetParam()->Value * GetStack());
}

bool UAddCriticalPercent::Activate()
{
	if (!Super::Activate())
		return false;

	GetTarget()->AddStat(ECharacterStatType::CRITICAL_PERCENT, GetParam()->Value);
	return true;
}

void UAddCriticalPercent::Deactivate()
{
	if (!IsValid()) return;

	GetTarget()->SubStat(ECharacterStatType::CRITICAL_PERCENT, GetParam()->Value * GetStack());
}

bool UAddCriticalDamagePercent::Activate()
{
	if (!Super::Activate())
		return false;

	GetTarget()->AddStat(ECharacterStatType::CRITICAL_DAMAGE_PERCENT, GetParam()->Value);
	return true;
}

void UAddCriticalDamagePercent::Deactivate()
{
	if (!IsValid()) return;

	GetTarget()->SubStat(ECharacterStatType::CRITICAL_DAMAGE_PERCENT, GetParam()->Value * GetStack());
}

bool UAddAttackPercent::Activate()
{
	if (!Super::Activate())
		return false;
	
	uint32 Attack = GetTarget()->GetStat(ECharacterStatType::ATTACK, true);
	Attack = static_cast<uint32>(Attack * GetParam()->Value * 0.01f);
	
	GetTarget()->AddStat(ECharacterStatType::ATTACK, Attack);
	return true;
}

void UAddAttackPercent::Deactivate()
{
	if (!IsValid()) return;

	uint32 Attack = GetTarget()->GetStat(ECharacterStatType::ATTACK, true);
	Attack = static_cast<uint32>(Attack * GetParam()->Value * 0.01f);
	Attack *= GetStack();

	GetTarget()->SubStat(ECharacterStatType::ATTACK, Attack);
}
