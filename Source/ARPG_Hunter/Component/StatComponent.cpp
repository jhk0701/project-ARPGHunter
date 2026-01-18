// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/StatComponent.h"

UStatComponent::UStatComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

}

void UStatComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UStatComponent::TakeDamage(uint16 _damage)
{
	if (IsDead())
		return;

	if (Health < _damage)
		Health = 0;
	else
		Health -= _damage;

	OnTakeDamage.Broadcast(GetHealthPercent());
}

bool UStatComponent::TryUseStamina(uint16 _amount)
{
	if (Stamina < _amount)
		return false;

	Stamina -= _amount;
	
	OnUseStamina.Broadcast(GetStaminaPercent());

	return true;
}

