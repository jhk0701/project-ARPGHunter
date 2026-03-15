// Fill out your copyright notice in the Description page of Project Settings.


#include "Effect/Effect.h"
#include "Data/EffectData.h"
#include "Component/StatComponent.h"

uint32 UEffect::GetValue() const
{
	return BaseParam->Value + AddictiveValue;
}

float UEffect::GetDuration() const
{ 
	return BaseParam->Duration; 
}

float UEffect::GetRepeatInterval() const
{
	return BaseParam->RepeatInterval;
}

const TArray<TObjectPtr<class UEffectData>>& UEffect::GetEffectOnEvent() const
{
	return BaseParam->EffectsOnEvent;
}

uint8 UEffect::GetMaxStack() const
{
	return BaseParam->MaxStack;
}

bool UEffect::IsStackFull() const
{
	return Stack >= BaseParam->MaxStack;
}

void UEffect::AddStack() 
{
	Stack = FMath::Min<uint8>(Stack + 1, BaseParam->MaxStack);
}

bool URecoverHealth::Activate()
{
	if (Super::Activate() == false)
		return false;

	if (IsValid())
		GetTarget()->RecoverResource(ECharacterResourceType::HEALTH, GetValue());

	return true;
}

bool URecoverSkill::Activate()
{
	if (Super::Activate() == false)
		return false;
	
	if (IsValid())
		GetTarget()->RecoverResource(ECharacterResourceType::SKILL, GetValue());

	return true;
}

bool URecoverStamina::Activate()
{
	if (Super::Activate() == false)
		return false;
	
	if (IsValid())
		GetTarget()->RecoverResource(ECharacterResourceType::STAMINA, GetValue());

	return true;
}

bool UAddEffectUsingSkill::Activate()
{
	if (Super::Activate() == false)
		return false;

	if (!IsValid()) 
		return false;

	TWeakObjectPtr<UStatComponent> Target = GetTarget();

	// 입력한 Value만큼 Skill 수치 소모
	if (Target->TryUseResource(ECharacterResourceType::SKILL, GetValue()) == false)
		return false;

	for (TObjectPtr<UEffectData> data : GetEffectOnEvent())
		Target->ApplyEffect(data);

	return true;
}