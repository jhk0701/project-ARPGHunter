// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/MonsterSpawner.h"
#include "Components/BoxComponent.h"

// Sets default values
AMonsterSpawner::AMonsterSpawner()
{
	PrimaryActorTick.bCanEverTick = false;

	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawnArea"));
	SetRootComponent(BoxComp);

	BoxComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

// Called when the game starts or when spawned
void AMonsterSpawner::BeginPlay()
{
	Super::BeginPlay();

	SpawnMonster();
}

void AMonsterSpawner::SpawnMonster()
{
}
