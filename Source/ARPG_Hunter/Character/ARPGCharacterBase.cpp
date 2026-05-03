// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/ARPGCharacterBase.h"

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

void AARPGCharacterBase::HandleAttackNotify(uint8 _opt)
{
}

void AARPGCharacterBase::HitBy(const FHitInfo& _hitInfo)
{

}