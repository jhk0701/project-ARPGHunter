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

	_target->AddAttack(_param->Value);
}

void UAddAttack::Deactivate()
{
	if (!IsValid()) return;

	GetTarget()->SubAttack(GetParam()->Value);
}

void UAddDefense::Activate(UStatComponent* _target, FEffectParam* _param)
{
	Super::Activate(_target, _param);
	if (!IsValid()) return;

	_target->AddDefense(_param->Value);
}

void UAddDefense::Deactivate()
{
	if (!IsValid()) return;

	GetTarget()->SubDefense(GetParam()->Value);
}

void UAddCriticalPercent::Activate(UStatComponent* _target, FEffectParam* _param)
{
	Super::Activate(_target, _param);
	if (!IsValid()) return;

	_target->AddCritPer(_param->Value);
}

void UAddCriticalPercent::Deactivate()
{
	if (!IsValid()) return;

	GetTarget()->SubCritPer(GetParam()->Value);
}

void UAddCriticalDamagePercent::Activate(UStatComponent* _target, FEffectParam* _param)
{
	Super::Activate(_target, _param);
	if (!IsValid()) return;

	_target->AddCritDmg(_param->Value);
}

void UAddCriticalDamagePercent::Deactivate()
{
	if (!IsValid()) return;

	GetTarget()->SubCritDmg(GetParam()->Value);
}
