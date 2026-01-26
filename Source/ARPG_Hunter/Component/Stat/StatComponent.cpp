// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/Stat/StatComponent.h"
#include "Effect/Effect.h"

UStatComponent::UStatComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}
void UStatComponent::Init()
{
	Health = MaxHealth;
}

void UStatComponent::TakeDamage(uint16 _damage)
{
	if (IsDead())
		return;
	
	// 피격 발생
	bool bHitPredicate = false;
	OnHitEvent.Broadcast(bHitPredicate); // 피격 시 이벤트 델리게이트 호출
	
	if (bHitPredicate) // 결과 true인 경우 return
		return;

	if (Health < _damage)
		Health = 0;
	else
		Health -= _damage;

	OnHealthChanged.Broadcast(Health, MaxHealth);
}

void UStatComponent::RecoverHealth(uint16 _amount)
{
	if (Health == MaxHealth)
		return;

	Health = FMath::Min<uint16>(Health + _amount, MaxHealth);

	OnHealthChanged.Broadcast(Health, MaxHealth);
}

void UStatComponent::AddEffect(TObjectPtr<UEffect> _effect)
{ 
	// 이펙트 등록
	FTimerHandle& EffectTimer = MapEffect.Add(_effect);
	GetWorld()->GetTimerManager().SetTimer(
		EffectTimer, 
		[&]() { RemoveEffect(_effect); },
		_effect->GetDuration(), 
		false);
}

void UStatComponent::RemoveEffect(TObjectPtr<UEffect> _effect)
{
	MapEffect.Remove(_effect);
}
