// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/Stat/PlayerStatComponent.h"

void UPlayerStatComponent::Init()
{
	Super::Init();

	Stamina = MaxStamina;
	StartStaminaRecovery();

	Skill = 0;
}

void UPlayerStatComponent::StartStaminaRecovery()
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

bool UPlayerStatComponent::TryUseStamina(uint16 _amount)
{
	if (Stamina < _amount)
		return false;

	Stamina -= _amount;
	OnStaminaChanged.Broadcast(Stamina, MaxStamina);

	PauseAndRestartStaminaRecovery(StaminaRecoveryPauseTime);

	return true;
}

void UPlayerStatComponent::RecoverStamina(uint16 _amount)
{
	if (Stamina == MaxStamina)
		return;

	Stamina = FMath::Min<uint16>(Stamina + _amount, MaxStamina);

	OnStaminaChanged.Broadcast(Stamina, MaxStamina);
}

void UPlayerStatComponent::PauseAndRestartStaminaRecovery(float _pauseSecond)
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

bool UPlayerStatComponent::TryUseSkill(uint8 _amount)
{
	if (Skill < _amount)
		return false;

	Skill -= _amount;
	OnSkillChanged.Broadcast(Skill, MaxSkill);

	return true;
}

void UPlayerStatComponent::GainSkill(uint8 _amount)
{
	if (Skill == MaxSkill)
		return;

	Skill = FMath::Min<uint8>(MaxSkill, Skill + _amount);

	OnSkillChanged.Broadcast(Skill, MaxSkill);
}
