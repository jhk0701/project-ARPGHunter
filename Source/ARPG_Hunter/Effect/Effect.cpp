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

void URecoverHealth::Activate(UStatComponent* _target, FEffectContext* _context)
{
	Super::Activate(_target, _context);

	if (IsValid())
		_target->RecoverResource(ECharacterResourceType::HEALTH, _context->Param->Value);
}

void URecoverSkill::Activate(UStatComponent* _target, FEffectContext* _context)
{
	Super::Activate(_target, _context);
	
	if (IsValid())
		_target->RecoverResource(ECharacterResourceType::SKILL, _context->Param->Value);
}

void URecoverStamina::Activate(UStatComponent* _target, FEffectContext* _context)
{
	Super::Activate(_target, _context);
	
	if (IsValid())
		_target->RecoverResource(ECharacterResourceType::STAMINA, _context->Param->Value);
}

void UAddEffectUsingSkill::Activate(UStatComponent* _target, FEffectContext* _context)
{
	Super::Activate(_target, _context);

	if (!IsValid()) 
		return;
	// 입력한 Value만큼 Skill 수치 소모
	if (_target->TryUseResource(ECharacterResourceType::SKILL, _context->Param->Value) == false)
		return;

	for (TObjectPtr<UEffectData> data : _context->Param->EffectsOnEvent)
		_target->ApplyEffect(data);
}