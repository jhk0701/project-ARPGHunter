// Fill out your copyright notice in the Description page of Project Settings.


#include "Effect/TermEffect/TermEffect.h"
#include "Data/EffectData.h"

#include "Define/Debug.h"

bool UTermEffect::Activate()
{
	if (!IsValid())
		return false;

	// 효과의 유효기간 타이머 설정
	// 타이머 경과 이후, Deactivate 호출
	if (false == GetTarget()->RegisterEffect(this))
		return false;

	return Super::Activate();
}

void UTermEffect::Deactivate(){}

bool UAddAttack::Activate()
{
	if (!Super::Activate()) 
		return false;

	GetTarget()->AddStat(ECharacterStatType::ATTACK, GetValue());
	return true;
}

void UAddAttack::Deactivate()
{
	if (!IsValid()) return;

	GetTarget()->SubStat(ECharacterStatType::ATTACK, GetValue() * GetStack());
}

bool UAddDefense::Activate()
{
	if (!Super::Activate())
		return false;

	GetTarget()->AddStat(ECharacterStatType::DEFENSE, GetValue());
	return true;
}

void UAddDefense::Deactivate()
{
	if (!IsValid()) return;

	GetTarget()->SubStat(ECharacterStatType::DEFENSE, GetValue() * GetStack());
}

bool UAddCriticalPercent::Activate()
{
	if (false == Super::Activate())
		return false;

	GetTarget()->AddStat(ECharacterStatType::CRITICAL_PERCENT, GetValue());
	return true;
}

void UAddCriticalPercent::Deactivate()
{
	if (!IsValid()) return;

	GetTarget()->SubStat(ECharacterStatType::CRITICAL_PERCENT, GetValue() * GetStack());
}

bool UAddCriticalDamagePercent::Activate()
{
	if (false == Super::Activate())
		return false;

	GetTarget()->AddStat(ECharacterStatType::CRITICAL_DAMAGE_PERCENT, GetValue());
	return true;
}

void UAddCriticalDamagePercent::Deactivate()
{
	if (!IsValid()) return;

	GetTarget()->SubStat(ECharacterStatType::CRITICAL_DAMAGE_PERCENT, GetValue() * GetStack());
}

bool UAddAttackPercent::Activate()
{
	if (false == Super::Activate())
		return false;
	
	uint32 Attack = GetTarget()->GetStat(ECharacterStatType::ATTACK, true);
	Attack = static_cast<uint32>(Attack * GetValue() * 0.01f);
	
	GetTarget()->AddStat(ECharacterStatType::ATTACK, Attack);
	return true;
}

void UAddAttackPercent::Deactivate()
{
	if (!IsValid()) return;

	uint32 Attack = GetTarget()->GetStat(ECharacterStatType::ATTACK, true);
	Attack = static_cast<uint32>(Attack * GetValue() * 0.01f);
	Attack *= GetStack();

	GetTarget()->SubStat(ECharacterStatType::ATTACK, Attack);
}

bool USubDefensePercent::Activate()
{
	if (false == Super::Activate())
		return false;

	uint32 Defense = GetTarget()->GetStat(ECharacterStatType::DEFENSE, true);
	Defense = static_cast<uint32>(Defense * GetValue() * 0.01f);

	GetTarget()->DebuffStat(ECharacterStatType::DEFENSE, Defense);
	return true;
}

void USubDefensePercent::Deactivate()
{
	if (!IsValid()) return;

	uint32 Defense = GetTarget()->GetStat(ECharacterStatType::DEFENSE, true);
	Defense = static_cast<uint32>(Defense * GetValue() * 0.01f);
	Defense *= GetStack();

	GetTarget()->RemoveDebuffStat(ECharacterStatType::DEFENSE, Defense);
}
