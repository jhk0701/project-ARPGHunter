// Fill out your copyright notice in the Description page of Project Settings.


#include "Effect/PeriodicalEffect/PeriodicalEffect.h"
#include "Data/EffectData.h"
#include "Component/StatComponent.h"

bool UPeriodicalEffect::Activate()
{
	if (!IsValid())
		return false;

	if (!Super::Activate())
		return false;

	// 내부적 호출 사이클용 타이머 설정
	GetTarget()->GetWorld()->GetTimerManager().SetTimer(
		RepeatTimer,
		this,
		&UPeriodicalEffect::RepeatedActivate,
		GetParam()->RepeatInterval,
		true
	);

	return true;
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
		GetTarget()->RecoverResource(ECharacterResourceType::HEALTH, GetParam()->Value);
}

void URepeatlyRecoverStamina::RepeatedActivate()
{
	if (IsValid())
		GetTarget()->RecoverResource(ECharacterResourceType::STAMINA, GetParam()->Value);
}

void URepeatlyRecoverSkill::RepeatedActivate()
{
	if (IsValid())
		GetTarget()->RecoverResource(ECharacterResourceType::SKILL, GetParam()->Value);
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
