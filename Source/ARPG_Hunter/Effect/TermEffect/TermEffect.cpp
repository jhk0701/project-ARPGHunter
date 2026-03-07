// Fill out your copyright notice in the Description page of Project Settings.


#include "Effect/TermEffect/TermEffect.h"
#include "Data/EffectData.h"

void UTermEffect::Activate(UStatComponent* _target, FEffectContext* _context)
{
	Super::Activate(_target, _context);

	if (_target == nullptr || _context == nullptr)
		return;

	// 효과의 유효기간 타이머 설정
	// 타이머 경과 이후, Deactivate 호출
	_target->RegisterEffect(this); 
}

void UTermEffect::Deactivate(){}

void UAddAttack::Activate(UStatComponent* _target, FEffectContext* _context)
{
	Super::Activate(_target, _context);
	if (!IsValid()) return;

	_target->AddStat(ECharacterStatType::ATTACK, GetParam()->Value);
}

void UAddAttack::Deactivate()
{
	if (!IsValid()) return;

	GetTarget()->SubStat(ECharacterStatType::ATTACK, GetParam()->Value * GetStack());
}

void UAddDefense::Activate(UStatComponent* _target, FEffectContext* _context)
{
	Super::Activate(_target, _context);
	if (!IsValid()) return;

	GetTarget()->AddStat(ECharacterStatType::DEFENSE, GetParam()->Value);
}

void UAddDefense::Deactivate()
{
	if (!IsValid()) return;

	GetTarget()->SubStat(ECharacterStatType::DEFENSE, GetParam()->Value * GetStack());
}

void UAddCriticalPercent::Activate(UStatComponent* _target, FEffectContext* _context)
{
	Super::Activate(_target, _context);
	if (!IsValid()) return;

	GetTarget()->AddStat(ECharacterStatType::CRITICAL_PERCENT, GetParam()->Value);
}

void UAddCriticalPercent::Deactivate()
{
	if (!IsValid()) return;

	GetTarget()->SubStat(ECharacterStatType::CRITICAL_PERCENT, GetParam()->Value * GetStack());
}

void UAddCriticalDamagePercent::Activate(UStatComponent* _target, FEffectContext* _context)
{
	Super::Activate(_target, _context);
	if (!IsValid()) return;

	GetTarget()->AddStat(ECharacterStatType::CRITICAL_DAMAGE_PERCENT, GetParam()->Value);
}

void UAddCriticalDamagePercent::Deactivate()
{
	if (!IsValid()) return;

	GetTarget()->SubStat(ECharacterStatType::CRITICAL_DAMAGE_PERCENT, GetParam()->Value * GetStack());
}

void UAddAttackPercent::Activate(UStatComponent* _target, FEffectContext* _context)
{
	Super::Activate(_target, _context);
	if (!IsValid()) return;
	
	uint32 Attack = GetTarget()->GetStat(ECharacterStatType::ATTACK, true);
	Attack = static_cast<uint32>(Attack * GetParam()->Value * 0.01f);
	GetTarget()->AddStat(ECharacterStatType::ATTACK, Attack);
}

void UAddAttackPercent::Deactivate()
{
	if (!IsValid()) return;

	uint32 Attack = GetTarget()->GetStat(ECharacterStatType::ATTACK, true);
	Attack = static_cast<uint32>(Attack * (GetParam()->Value * GetStack() * 0.01f));
	GetTarget()->SubStat(ECharacterStatType::ATTACK, Attack);
}
