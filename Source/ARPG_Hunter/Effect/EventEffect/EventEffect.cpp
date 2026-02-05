// Fill out your copyright notice in the Description page of Project Settings.


#include "Effect/EventEffect/EventEffect.h"
#include "Data/EffectData.h"
#include "Component/StatComponent.h"

void UEventEffect::Activate(UStatComponent* _target, FEffectContext* _context)
{
	Super::Activate(_target, _context); // 상위 TermEffect에서 타이머를 설정완료

	if (!IsValid())
		return;

	EventHandle = Subscribe(); // 어디에 구독할지는 하위에서 결정
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


void UInvincibleEffect::OnHitEvent(EHitOption& _hitOption, uint32& _damage)
{
	if (!IsValid())
		return;

	// 무적 버프 중, 피격 발생
	_hitOption = EHitOption::IMMUNE_HIT;
}

void UJustDodgeEffect::OnHitEvent(EHitOption& _hitOption, uint32& _damage)
{
	// 버프를 가진 상태에서 피격을 당함
	if (!IsValid())
		return;

	for (TObjectPtr<UEffectData>& subEffect : GetParam()->EffectsOnEvent)
		GetTarget()->ApplyEffect(subEffect);

	GetTarget()->RemoveEffect(this);
}

void USuperArmorEffect::OnHitEvent(EHitOption& _hitOption, uint32& _damage)
{
	if (!IsValid())
		return;

	// 슈퍼 아머 적용
	if (_hitOption < EHitOption::IMMUNE_STIFFEN)
		_hitOption = EHitOption::IMMUNE_STIFFEN; // 경직 면역
	// 데미지 경감
	_damage = static_cast<uint32>(_damage * 0.2f);
}
