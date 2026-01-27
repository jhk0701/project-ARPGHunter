// Fill out your copyright notice in the Description page of Project Settings.


#include "Effect/RepeatedActivateEffect/RepeatedActivateEffect.h"
#include "Define/Struct.h"
#include "Component/StatComponent.h"

void URepeatedActivateEffect::Activate(UStatComponent* _target, FEffectParam* _param)
{
	Super::Activate(_target, _param);

	if (_target == nullptr || _param == nullptr)
		return;

	_target->GetWorld()->GetTimerManager().SetTimer(
		RepeatTimer,
		this,
		&URepeatedActivateEffect::RepeatedActivate,
		_param->RepeatInterval,
		true
	);
}


void URepeatedActivateEffect::Deactivate()
{
	if (!IsValid()) return;

	UStatComponent* StatComp = GetTarget().Get();
	FTimerManager& TimerManager = StatComp->GetWorld()->GetTimerManager();

	if (TimerManager.IsTimerActive(RepeatTimer))
		TimerManager.ClearTimer(RepeatTimer);
}


// 리소스 회복
void URecoverHealth::RepeatedActivate()
{
	if (!IsValid()) 
		return;

	GetTarget()->RecoverHealth(GetParam()->Value);
}

void URecoverStamina::RepeatedActivate()
{
	if (!IsValid())
		return;

	GetTarget()->RecoverStamina(GetParam()->Value);
}

void URecoverSkill::RepeatedActivate()
{
	if (!IsValid())
		return;

	GetTarget()->RecoverSkill(GetParam()->Value);
}

// 리소스 데미지
void UDamageHealth::RepeatedActivate()
{
	if (!IsValid())
		return;

	GetTarget()->TakeDamage(GetParam()->Value);
}

void UDamageStamina::RepeatedActivate()
{
	if (!IsValid())
		return;

	GetTarget()->TakeStaminaDamage(GetParam()->Value);
}
