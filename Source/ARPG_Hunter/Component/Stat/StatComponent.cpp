// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/Stat/StatComponent.h"

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
