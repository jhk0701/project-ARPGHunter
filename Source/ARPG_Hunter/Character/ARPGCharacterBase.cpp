// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/ARPGCharacterBase.h"

#include "Define/Enum.h"
#include "Core/GameMode/CombatGameMode.h"
#include "Component/StatComponent.h"

// Sets default values
AARPGCharacterBase::AARPGCharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;

	StatComp = CreateDefaultSubobject<UStatComponent>(TEXT("StatComp"));
}

// Called when the game starts or when spawned
void AARPGCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
	StatComp->OnDead.AddUObject(this, &AARPGCharacterBase::OnDead);
}

void AARPGCharacterBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	StatComp->Clear();
}

void AARPGCharacterBase::ApplyEffect(const FApplyEffectParam& _param)
{
	if (IsDead())
		return;

	StatComp->ApplyEffect(_param);
}

bool AARPGCharacterBase::IsDead() const
{
	return StatComp->IsDead();
}

uint32 AARPGCharacterBase::HitBy(const FHitInfo& _hitInfo)
{
	if (IsDead())
		return 0;

	TWeakObjectPtr<UStatComponent> Stat = GetStatComp();
	uint32 Damage = _hitInfo.bIgnoreDefense ?
		_hitInfo.Damage :
		ACombatGameMode::CalculateDefense(_hitInfo.Damage, Stat->GetStat(ECharacterStatType::DEFENSE));

	Stat->TakeDamage(Damage, [this]() { OnCharacterHit(); });

	return Damage;
}