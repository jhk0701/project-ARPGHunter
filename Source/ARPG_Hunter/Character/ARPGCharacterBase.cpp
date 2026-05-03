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
	
}

void AARPGCharacterBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	StatComp->Clear();
}

void AARPGCharacterBase::HitBy(const FHitInfo& _hitInfo)
{
}

void AARPGCharacterBase::ApplyEffect(const FApplyEffectParam& _param)
{
}

void AARPGCharacterBase::HandleAttackNotify(uint8 _opt)
{
}
