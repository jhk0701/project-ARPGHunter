// Fill out your copyright notice in the Description page of Project Settings.


#include "Effect/TermEffect/TermEffect.h"
#include "Data/EffectData.h"

void UTermEffect::Activate(UStatComponent* _target, FEffectParam* _param)
{
	Super::Activate(_target, _param);

	if (_target == nullptr || _param == nullptr)
		return;

	// 효과의 유효기간 타이머 설정
	// 타이머 경과 이후, Deactivate 호출
	_target->RegisterEffect(this); 
}

void UTermEffect::Deactivate()
{
}

void UAddAttack::Activate(UStatComponent* _target, FEffectParam* _param)
{
	Super::Activate(_target, _param);
	if (!IsValid()) return;

	_target->AddStat(ECharacterStatType::ATTACK, _param->Value);
}

void UAddAttack::Deactivate()
{
	if (!IsValid()) return;

	GetTarget()->SubStat(ECharacterStatType::ATTACK, GetParam()->Value);
}

void UAddDefense::Activate(UStatComponent* _target, FEffectParam* _param)
{
	Super::Activate(_target, _param);
	if (!IsValid()) return;

	GetTarget()->AddStat(ECharacterStatType::DEFENSE, _param->Value);
}

void UAddDefense::Deactivate()
{
	if (!IsValid()) return;

	GetTarget()->SubStat(ECharacterStatType::DEFENSE, GetParam()->Value);
}

void UAddCriticalPercent::Activate(UStatComponent* _target, FEffectParam* _param)
{
	Super::Activate(_target, _param);
	if (!IsValid()) return;

	GetTarget()->AddStat(ECharacterStatType::CRITICAL_PERCENT, _param->Value);
}

void UAddCriticalPercent::Deactivate()
{
	if (!IsValid()) return;

	GetTarget()->SubStat(ECharacterStatType::CRITICAL_PERCENT, GetParam()->Value);
}

void UAddCriticalDamagePercent::Activate(UStatComponent* _target, FEffectParam* _param)
{
	Super::Activate(_target, _param);
	if (!IsValid()) return;

	GetTarget()->AddStat(ECharacterStatType::CRITICAL_DAMAGE_PERCENT, _param->Value);
}

void UAddCriticalDamagePercent::Deactivate()
{
	if (!IsValid()) return;

	GetTarget()->SubStat(ECharacterStatType::CRITICAL_DAMAGE_PERCENT, GetParam()->Value);
}
