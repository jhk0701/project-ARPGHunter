// Fill out your copyright notice in the Description page of Project Settings.


#include "Effect/RepeatedActivateEffect/RepeatedActivateEffect.h"
#include "Component/StatComponent.h"

void URepeatedActivateEffect::Activate(TWeakObjectPtr<UStatComponent> _target, FEffectParam* _param)
{
	Super::Activate(_target, _param);

	if (GetTarget().IsValid() == false)
		return;

	UStatComponent* StatComp = GetTarget().Get();
	StatComp->GetWorld()->GetTimerManager().SetTimer(
		RepeatTimer,
		this,
		&URepeatedActivateEffect::RepeatedActivate,
		_param->RepeatInterval,
		true
	);
}

void URepeatedActivateEffect::Deactivate()
{
	if (GetTarget().IsValid() == false)
		return;

	UStatComponent* StatComp = GetTarget().Get();
	FTimerManager& TimerManager = StatComp->GetWorld()->GetTimerManager();
	if (TimerManager.IsTimerActive(RepeatTimer))
		TimerManager.ClearTimer(RepeatTimer);
}

void URepeatedActivateEffect::RepeatedActivate()
{
	// 자식 클래스에서 구체적인 동작 위임
}
