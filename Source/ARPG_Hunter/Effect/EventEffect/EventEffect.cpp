// Fill out your copyright notice in the Description page of Project Settings.


#include "Effect/EventEffect/EventEffect.h"

#include "Interface/Effectable.h"
#include "Data/EffectData.h"
#include "Component/Stat/StatComponent.h"

bool UEventEffect::Activate()
{
	if (!IsValid())
		return false;

	if(!Super::Activate()) // 상위 TermEffect에서 타이머를 설정완료
		return false;

	EventHandle = Subscribe(); // 어디에 구독할지는 하위에서 결정

	return true;
}

void UEventEffect::Deactivate()
{
	if (!IsValid() || EventHandle.IsValid() == false) return;

	// 타이머 종료 시, 델리게이트 핸들 제거
	GetTarget()->OnHitEvent.Remove(EventHandle);
	EventHandle.Reset();
}


FDelegateHandle UOnHitEffect::Subscribe()
{
	return GetTarget()->OnHitEvent.AddUObject(this, &UOnHitEffect::OnHitEvent);
}


void UInvincibleEffect::OnHitEvent(uint8& _hitBit, uint32& _damage)
{
	if (!IsValid())
		return;

	// 무적 버프 중, 피격 발생
	_hitBit |= static_cast<uint8>(EHitOption::IMMUNE_HIT); // 피격 면역 적용
}

void UJustDodgeEffect::OnHitEvent(uint8& _hitBit, uint32& _damage)
{
	// 버프를 가진 상태에서 피격을 당함
	if (!IsValid())
		return;

	for (TObjectPtr<UEffectData> subEffect : GetTargetEffect())
	{
		FApplyEffectParam Param;
		Param.Subject = GetSubject();
		Param.EffectData = subEffect;

		GetTarget()->ApplyEffect(Param);
	}

	GetTarget()->RemoveEffect(this);
}

void USuperArmorEffect::OnHitEvent(uint8& _hitBit, uint32& _damage)
{
	if (!IsValid())
		return;

	// 슈퍼 아머 적용
	_hitBit |= static_cast<uint8>(EHitOption::IMMUNE_STIFFEN); // 경직 면역 적용
	_damage *= (1.0f - static_cast<float>(GetValue()) * 0.01f); // 데미지 경감
}
