// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/StatComponent.h"

UStatComponent::UStatComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UStatComponent::Init()
{
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

void UStatComponent::PauseAndRestartStaminaRecovery(float _pauseSecond)
{
	FTimerManager& TimerManager = GetWorld()->GetTimerManager();

	if (TimerManager.IsTimerActive(StaminaRecoveryTimer))
		TimerManager.ClearTimer(StaminaRecoveryTimer);

	TimerManager.SetTimer(StaminaRecoveryTimer,
		[this]() 
		{
			RecoverStamina(StaminaRecoveryPerSecond * StaminaRecoveryRate);
		},
		StaminaRecoveryRate,
		true,
		_pauseSecond
	);
}

void UStatComponent::TakeDamage(uint16 _damage)
{
	if (IsDead())
		return;

	if (Health < _damage)
		Health = 0;
	else
		Health -= _damage;

	OnTakeDamage.Broadcast(Health, MaxHealth);
}

bool UStatComponent::TryUseStamina(uint16 _amount)
{
	if (Stamina < _amount)
		return false;

	Stamina -= _amount;
	OnUseStamina.Broadcast(Stamina, MaxStamina);

	PauseAndRestartStaminaRecovery(StaminaRecoveryPauseTime);

	return true;
}

void UStatComponent::RecoverStamina(uint16 _amount)
{
	if (Stamina == MaxStamina)
		return;
	
	Stamina = FMath::Min<uint16>(Stamina + _amount, MaxStamina);

	OnUseStamina.Broadcast(Stamina, MaxStamina);
}

