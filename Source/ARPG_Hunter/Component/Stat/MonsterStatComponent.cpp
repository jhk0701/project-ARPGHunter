// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/Stat/MonsterStatComponent.h"

void UMonsterStatComponent::Init()
{
	Super::Init();
	Stagger = MaxStagger;
}

void UMonsterStatComponent::TakeStaggerDamage(uint16 _staggerDamage)
{
	if (IsStaggering())
		return;

	if (Stagger < _staggerDamage)
		Stagger = 0;
	else
		Stagger -= _staggerDamage;

	OnStaggerChanged.Broadcast(Stagger, MaxStagger);

	if (IsStaggering())
		StartStaggerRecover();
}

void UMonsterStatComponent::StartStaggerRecover()
{
	GetWorld()->GetTimerManager().SetTimer(StaggerRecoverTimer,
		this, &UMonsterStatComponent::RecoverStagger,
		StaggerRecoverTime, false);
}

void UMonsterStatComponent::RecoverStagger()
{
	Stagger = MaxStagger;
	OnStaggerChanged.Broadcast(Stagger, MaxStagger);
}
