// Fill out your copyright notice in the Description page of Project Settings.


#include "Effect/PeriodicalEffect/PeriodicalEffect.h"
#include "Data/EffectData.h"
#include "Component/StatComponent.h"

void UPeriodicalEffect::Activate(UStatComponent* _target, FEffectParam* _param)
{
	Super::Activate(_target, _param);

	if (_target == nullptr || _param == nullptr)
		return;

	// 내부적 호출 사이클용 타이머 설정
	_target->GetWorld()->GetTimerManager().SetTimer(
		RepeatTimer,
		this,
		&UPeriodicalEffect::RepeatedActivate,
		_param->RepeatInterval,
		true
	);
}

void UPeriodicalEffect::Deactivate()
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
	if (IsValid())
		GetTarget()->RecoverHealth(GetParam()->Value);
}

void URepeatlyRecoverStamina::RepeatedActivate()
{
	if (IsValid())
		GetTarget()->RecoverStamina(GetParam()->Value);
}

void URepeatlyRecoverSkill::RepeatedActivate()
{
	if (IsValid())
		GetTarget()->RecoverSkill(GetParam()->Value);
}

// 리소스 데미지
void URepeatlyDamageHealth::RepeatedActivate()
{
	if (IsValid())
		GetTarget()->TakeDamage(GetParam()->Value);
}

void URepeatlyDamageStamina::RepeatedActivate()
{
	if (IsValid())
		GetTarget()->TakeStaminaDamage(GetParam()->Value);
}
