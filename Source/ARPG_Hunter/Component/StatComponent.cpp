// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/StatComponent.h"
#include "Effect/Effect.h"

UStatComponent::UStatComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}
void UStatComponent::Init()
{
	Health = MaxHealth;
	Stamina = MaxStamina;
	Skill = 0;
	StartStaminaRecovery();
}

void UStatComponent::StartStaminaRecovery()
{
	GetWorld()->GetTimerManager().SetTimer(
		StaminaRecoveryTimer,
		[this]()
		{
			RecoverStamina(StaminaRecoveryPerSecond * StaminaRecoveryRate);
		},
		StaminaRecoveryRate,
		true
	);
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

bool UStatComponent::TryUseStamina(uint16 _amount)
{
	if (Stamina < _amount)
		return false;

	Stamina -= _amount;
	OnStaminaChanged.Broadcast(Stamina, MaxStamina);

	// 스태미나 소진 시, 페널티 시간 부여
	PauseAndRestartStaminaRecovery(IsStaggering() ? PenaltyTimeOnStaminaExhaustion : StaminaRecoveryPauseTime);

	return true;
}

void UStatComponent::TakeStaminaDamage(uint16 _damage)
{
	if (IsStaggering())
		return;

	if (Stamina < _damage)
		Stamina = 0;
	else
		Stamina -= _damage;

	OnStaminaChanged.Broadcast(Stamina, MaxStamina);
}

void UStatComponent::RecoverStamina(uint16 _amount)
{
	if (Stamina == MaxStamina)
		return;

	Stamina = FMath::Min<uint16>(Stamina + _amount, MaxStamina);

	OnStaminaChanged.Broadcast(Stamina, MaxStamina);
}

void UStatComponent::PauseAndRestartStaminaRecovery(float _pauseSecond)
{
	FTimerManager& TimerManager = GetWorld()->GetTimerManager();

	if (TimerManager.IsTimerActive(StaminaRecoveryTimer))
		TimerManager.ClearTimer(StaminaRecoveryTimer);

	TimerManager.SetTimer(StaminaRecoveryTimer,
		[this]()
		{
			if(IsStaggering())
				RecoverStamina(static_cast<uint16>(MaxStamina * 0.5f));

			RecoverStamina(StaminaRecoveryPerSecond * StaminaRecoveryRate);
		},
		StaminaRecoveryRate,
		true,
		_pauseSecond
	);
}

bool UStatComponent::TryUseSkill(uint8 _amount)
{
	if (Skill < _amount)
		return false;

	Skill -= _amount;
	OnSkillChanged.Broadcast(Skill, MaxSkill);

	return true;
}

void UStatComponent::RecoverSkill(uint8 _amount)
{
	if (Skill == MaxSkill)
		return;

	Skill = FMath::Min<uint8>(MaxSkill, Skill + _amount);

	OnSkillChanged.Broadcast(Skill, MaxSkill);
}


void UStatComponent::ApplyEffect(TSubclassOf<UEffect> _effectClass, FEffectParam* _effectParam)
{
	// 이펙트 등록
	TObjectPtr<UEffect> EffectInst = NewObject<UEffect>(this, _effectClass);
	EffectInst->Activate(this, _effectParam);

	FTimerHandle& EffectTimer = MapEffect.Add(EffectInst);

	GetWorld()->GetTimerManager().SetTimer(
		EffectTimer,
		[&]() { RemoveEffect(EffectInst); },
		EffectInst->GetDuration(),
		false);
}

void UStatComponent::RemoveEffect(TObjectPtr<UEffect> _effect)
{
	_effect->Deactivate();
	MapEffect.Remove(_effect);
}