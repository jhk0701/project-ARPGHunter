// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/StatComponent.h"
#include "Effect/Effect.h"

UStatComponent::UStatComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	for (uint8 i = 0; i < static_cast<uint8>(ECharacterStatType::END); ++i)
	{
		ECharacterStatType type = static_cast<ECharacterStatType>(i);

		if (!Stat.Find(type))
			Stat.Add(type);

		Stat[type] = 100;

		if(!EffectedStat.Find(type))
			EffectedStat.Add(type);

		EffectedStat[type] = 0;
	}

	for (uint8 i = 0; i < static_cast<uint8>(ECharacterResourceType::END); ++i)
	{
		ECharacterResourceType type = static_cast<ECharacterResourceType>(i);

		if (!Resource.Find(type))
			Resource.Add(type);
	}
}

void UStatComponent::Init(const TMap<ECharacterStatType, uint32>& _initStat)
{
	Stat = _initStat;

	Resource[ECharacterResourceType::HEALTH].Init(Stat[ECharacterStatType::HEALTH]);
	Resource[ECharacterResourceType::STAMINA].Init(Stat[ECharacterStatType::STAMINA]);
	Resource[ECharacterResourceType::SKILL].Init(Stat[ECharacterStatType::SKILL], false);

	StartStaminaRecovery();
}

void UStatComponent::Clear()
{
	FTimerManager& TimerManager = GetWorld()->GetTimerManager();

	if (TimerManager.IsTimerActive(StaminaRecoveryTimer))
		TimerManager.ClearTimer(StaminaRecoveryTimer);

	for (TPair<TObjectPtr<UEffect>, FTimerHandle>& pair : MapEffect)
	{
		if (TimerManager.IsTimerActive(pair.Value))
			TimerManager.ClearTimer(pair.Value);
	}
}

void UStatComponent::StartStaminaRecovery()
{
	GetWorld()->GetTimerManager().SetTimer(
		StaminaRecoveryTimer,
		[this]()
		{
			RecoverResource(ECharacterResourceType::STAMINA, StaminaRecoveryPerSecond * StaminaRecoveryRate);
		},
		StaminaRecoveryRate,
		true
	);
}

bool UStatComponent::TryUseResource(ECharacterResourceType _type, uint32 _amount)
{
	FCharacterResource* pResource = &Resource[_type];

	if (pResource->Value < _amount)
		return false;

	pResource->Value -= _amount;
	pResource->InvokeDelegate();

	return true;
}

void UStatComponent::RecoverResource(ECharacterResourceType _type, uint32 _amount)
{
	FCharacterResource* pResource = &Resource[_type];

	if (pResource->Value == pResource->MaxValue)
		return;

	pResource->Value = FMath::Min<uint32>(pResource->Value + _amount, pResource->MaxValue);
	pResource->InvokeDelegate();
}

bool UStatComponent::TryUseStamina(uint32 _amount)
{
	bool bIsUsed = TryUseResource(ECharacterResourceType::STAMINA, _amount);

	// 스태미나 소진 시, 페널티 시간 부여
	if (bIsUsed)
		PauseAndRestartStaminaRecovery(IsStaggering() ? PenaltyTimeOnStaminaExhaustion : StaminaRecoveryPauseTime);

	return bIsUsed;
}

bool UStatComponent::TakeDamage(uint32 _damage)
{
	if (IsDead())
		return false;
	
	// 피격 발생
	bool bHitCanceled = false;
	OnHitEvent.Broadcast(bHitCanceled); // 피격 시 이벤트 델리게이트 호출
	
	if (bHitCanceled)
		return false;

	if (TryUseResource(ECharacterResourceType::HEALTH, _damage) == false)
	{
		Resource[ECharacterResourceType::HEALTH].Value = 0;
		Resource[ECharacterResourceType::HEALTH].InvokeDelegate();
	}

	return true;
}

void UStatComponent::TakeStaminaDamage(uint32 _damage)
{
	if (IsStaggering())
		return;

	if (TryUseResource(ECharacterResourceType::STAMINA, _damage) == false)
	{
		Resource[ECharacterResourceType::STAMINA].Value = 0;
		Resource[ECharacterResourceType::STAMINA].InvokeDelegate();
	}
}


void UStatComponent::PauseAndRestartStaminaRecovery(float _pauseSecond)
{
	FTimerManager& TimerManager = GetWorld()->GetTimerManager();

	if (TimerManager.IsTimerActive(StaminaRecoveryTimer))
		TimerManager.ClearTimer(StaminaRecoveryTimer);

	TimerManager.SetTimer(StaminaRecoveryTimer,
		[this]()
		{
			FCharacterResource* pStamina = &Resource[ECharacterResourceType::STAMINA];

			if (IsStaggering())
				RecoverResource(ECharacterResourceType::STAMINA, pStamina->MaxValue * 0.5f);
			else
				RecoverResource(ECharacterResourceType::STAMINA, StaminaRecoveryPerSecond * StaminaRecoveryRate);
		},
		StaminaRecoveryRate,
		true,
		_pauseSecond
	);
}

void UStatComponent::ApplyEffect(TSubclassOf<UEffect> _effectClass, FEffectParam* _effectParam)
{
	TObjectPtr<UEffect> EffectInst = NewObject<UEffect>(this, _effectClass);
	EffectInst->Activate(this, _effectParam);
}

void UStatComponent::RegisterEffect(TObjectPtr<UEffect> _effect)
{
	// 이펙트 등록
	FTimerHandle& EffectTimer = MapEffect.Add(_effect);
	GetWorld()->GetTimerManager().SetTimer(
		EffectTimer,
		[this, _effect]() { RemoveEffect(_effect); },
		_effect->GetDuration(),
		false);
}

void UStatComponent::RemoveEffect(TObjectPtr<UEffect> _effect)
{
	if (MapEffect.Find(_effect) == nullptr)
		return;

	FTimerManager& TimerManager = GetWorld()->GetTimerManager();
	if (TimerManager.IsTimerActive(MapEffect[_effect]))
		TimerManager.ClearTimer(MapEffect[_effect]);

	_effect->Deactivate();
	MapEffect.Remove(_effect);
}