// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/MonsterBase.h"
#include "Component/StatComponent.h"

AMonsterBase::AMonsterBase()
{ 	
	PrimaryActorTick.bCanEverTick = false;

	StatComp = CreateDefaultSubobject<UStatComponent>(TEXT("StatComp"));
}

// Called when the game starts or when spawned
void AMonsterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

//// Called every frame
//void AMonsterBase::Tick(float DeltaTime)
//{
//	Super::Tick(DeltaTime);
//
//}
//
//// Called to bind functionality to input
//void AMonsterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
//{
//	Super::SetupPlayerInputComponent(PlayerInputComponent);
//
//}

