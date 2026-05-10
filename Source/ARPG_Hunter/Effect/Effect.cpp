// Fill out your copyright notice in the Description page of Project Settings.


#include "Effect/Effect.h"

#include "Interface/Effectable.h"
#include "Interface/Hitable.h"
#include "Data/EffectData.h"
#include "Component/Stat/StatComponent.h"

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

const TArray<TObjectPtr<class UEffectData>>& UEffect::GetTargetEffect() const
{
	return BaseParam->TargetEffects;
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
	if (false == Super::Activate() || false == IsValid())
		return false;

	TWeakObjectPtr<UStatComponent> Target = GetTarget();

	// 입력한 Value만큼 Skill 수치 소모
	if (Target->TryUseResource(ECharacterResourceType::SKILL, GetValue()) == false)
		return false;

	for (TObjectPtr<UEffectData> data : GetTargetEffect())
	{
		FApplyEffectParam Param;
		Param.Subject = GetSubject();
		Param.EffectData = data;
		
		Target->ApplyEffect(Param);
	}

	return true;
}

bool UDamageUsingEffect::Activate()
{
	if (false == Super::Activate() || false == IsValid())
		return false;

	TWeakObjectPtr<AActor> SubjectActor = GetSubject();
	if (false == SubjectActor.IsValid())
		return false;

	const TArray<TObjectPtr<UEffectData>>& TargetEffects = GetTargetEffect();

	uint8 TotalStack = 0;
	for (TObjectPtr<UEffectData> TargetEffect : TargetEffects)
	{
		TWeakObjectPtr<UEffect> Effect = GetTarget()->GetAppliedEffect(TargetEffect);
		if (false == Effect.IsValid())
			continue;

		TotalStack += Effect->GetStack();

		// 타겟 효과 제거
		GetTarget()->RemoveEffect(Effect.Get());
	}

	uint32 SubjectAttack = 0;
	if (IEffectable* Effectable = Cast<IEffectable>(SubjectActor))
		SubjectAttack = Effectable->GetStatComp()->GetStat(ECharacterStatType::ATTACK);

	uint32 Damage = SubjectAttack * GetValue() * 0.01f;
	Damage *= TotalStack;

	if (IHitable* Hitable = Cast<IHitable>(GetTarget()->GetOwner()))
	{
		FHitInfo HitInfo;
		HitInfo.Attacker = SubjectActor;
		HitInfo.Damage = Damage;
		HitInfo.bIgnoreDefense = true; // 효과 데미지는 방어력 무시

		Hitable->HitBy(HitInfo);
	}

	return true;
}
