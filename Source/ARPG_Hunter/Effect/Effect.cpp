// Fill out your copyright notice in the Description page of Project Settings.


#include "Effect/Effect.h"
#include "Data/EffectData.h"
#include "Component/StatComponent.h"

float UEffect::GetDuration() 
{ 
	return Param->Duration; 
}

void URecoverHealth::Activate(UStatComponent* _target, FEffectParam* _param)
{
	Super::Activate(_target, _param);

	if (IsValid())
		_target->RecoverHealth(_param->Value);
}

void URecoverSkill::Activate(UStatComponent* _target, FEffectParam* _param)
{
	Super::Activate(_target, _param);
	
	if (IsValid())
		_target->RecoverSkill(_param->Value);
}

void URecoverStamina::Activate(UStatComponent* _target, FEffectParam* _param)
{
	Super::Activate(_target, _param);
	
	if (IsValid())
		_target->RecoverStamina(_param->Value);
}

void UAddEffectUsingSkill::Activate(UStatComponent* _target, FEffectParam* _param)
{
	Super::Activate(_target, _param);

	if (!IsValid()) 
		return;
	// 입력한 Value만큼 Skill 수치 소모
	if (_target->TryUseSkill(_param->Value) == false)
		return;

	for (TObjectPtr<UEffectData> data : _param->EffectsOnEvent)
		_target->ApplyEffect(data->Effect, &data->Param);
}