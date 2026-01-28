// Fill out your copyright notice in the Description page of Project Settings.


#include "Effect/EventEffect/EventEffect.h"
#include "Data/EffectData.h"
#include "Component/StatComponent.h"

void UEventEffect::Activate(UStatComponent* _target, FEffectParam* _param)
{
	Super::Activate(_target, _param); // 상위 TermEffect에서 타이머를 설정완료

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


void UInvincibleEffect::OnHitEvent(bool& _outbCancleHit)
{
	// 무적 버프 중, 피격 발생
	_outbCancleHit = true; // 피격 무효 처리
}

void UJustDodgeEffect::OnHitEvent(bool& _outbCancleHit)
{
	// 버프를 가진 상태에서 피격을 당함
	if (!IsValid())
		return;

	for (TObjectPtr<UEffectData>& subEffect : GetParam()->EffectsOnEvent)
		GetTarget()->ApplyEffect(subEffect->Effect, &subEffect->Param);

	GetTarget()->RemoveEffect(this);
}
