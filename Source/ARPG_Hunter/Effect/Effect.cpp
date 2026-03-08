// Fill out your copyright notice in the Description page of Project Settings.


#include "Effect/Effect.h"
#include "Data/EffectData.h"
#include "Component/StatComponent.h"

float UEffect::GetDuration()
{ 
	return BaseParam->Duration; 
}

uint8 UEffect::GetMaxStack()
{
	return BaseParam->MaxStack;
}

bool UEffect::IsStackFull()
{
	return Stack == BaseParam->MaxStack;
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
		GetTarget()->RecoverResource(ECharacterResourceType::HEALTH, GetParam()->Value);

	return true;
}

bool URecoverSkill::Activate()
{
	if (Super::Activate() == false)
		return false;
	
	if (IsValid())
		GetTarget()->RecoverResource(ECharacterResourceType::SKILL, GetParam()->Value);

	return true;
}

bool URecoverStamina::Activate()
{
	if (Super::Activate() == false)
		return false;
	
	if (IsValid())
		GetTarget()->RecoverResource(ECharacterResourceType::STAMINA, GetParam()->Value);

	return true;
}

bool UAddEffectUsingSkill::Activate()
{
	if (Super::Activate() == false)
		return false;

	if (!IsValid()) 
		return false;

	TWeakObjectPtr<UStatComponent> Target = GetTarget();
	FEffectParam* Param = GetParam();

	// 입력한 Value만큼 Skill 수치 소모
	if (Target->TryUseResource(ECharacterResourceType::SKILL, Param->Value) == false)
		return false;

	for (TObjectPtr<UEffectData> data : Param->EffectsOnEvent)
		Target->ApplyEffect(data);

	return true;
}