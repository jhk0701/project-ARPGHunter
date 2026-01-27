// Fill out your copyright notice in the Description page of Project Settings.


#include "Effect/RepeatlyActivateEffect/RepeatlyActivateEffect.h"
#include "Define/Struct.h"
#include "Component/StatComponent.h"

void URepeatlyActivateEffect::Activate(UStatComponent* _target, FEffectParam* _param)
{
	Super::Activate(_target, _param);

	if (_target == nullptr || _param == nullptr)
		return;

	_target->RegisterEffect(this);

	_target->GetWorld()->GetTimerManager().SetTimer(
		RepeatTimer,
		this,
		&URepeatlyActivateEffect::RepeatedActivate,
		_param->RepeatInterval,
		true
	);
}

void URepeatlyActivateEffect::Deactivate()
{
	if (!IsValid()) return;

	UStatComponent* StatComp = GetTarget().Get();
	FTimerManager& TimerManager = StatComp->GetWorld()->GetTimerManager();

	if (TimerManager.IsTimerActive(RepeatTimer))
		TimerManager.ClearTimer(RepeatTimer);
}


// 리소스 회복
void URepeatlyRecoverHealth::RepeatedActivate()
{
	if (!IsValid()) 
		return;

	GetTarget()->RecoverHealth(GetParam()->Value);
}

void URepeatlyRecoverStamina::RepeatedActivate()
{
	if (!IsValid())
		return;

	GetTarget()->RecoverStamina(GetParam()->Value);
}

void URepeatlyRecoverSkill::RepeatedActivate()
{
	if (!IsValid())
		return;

	GetTarget()->RecoverSkill(GetParam()->Value);
}

// 리소스 데미지
void URepeatlyDamageHealth::RepeatedActivate()
{
	if (!IsValid())
		return;

	GetTarget()->TakeDamage(GetParam()->Value);
}

void URepeatlyDamageStamina::RepeatedActivate()
{
	if (!IsValid())
		return;

	GetTarget()->TakeStaminaDamage(GetParam()->Value);
}
