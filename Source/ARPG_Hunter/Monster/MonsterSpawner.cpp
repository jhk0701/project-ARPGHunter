// Fill out your copyright notice in the Description page of Project Settings.

#include "Monster/MonsterSpawner.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetMathLibrary.h"

#include "Monster/MonsterBase.h"
#include "Subsystem/DataManager/DataManager.h"
#include "Data/MonsterData.h"

#include "Define/Debug.h"

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
	UDataManager* DataManager = GetGameInstance()->GetSubsystem<UDataManager>();
	for (const FName& id : MonsterIDs)
	{
		FVector Loc = UKismetMathLibrary::RandomPointInBoundingBox(GetActorLocation(), BoxComp->GetScaledBoxExtent());

		FMonsterData* MonsterData = DataManager->GetMonsterData(id);

		// 생성 후 즉시 ID 부여
		FActorSpawnParameters param;
		param.CustomPreSpawnInitalization = 
			[&id](AActor* _inst) 
			{
				AMonsterBase* Inst = Cast<AMonsterBase>(_inst);
				Inst->SetID(id);
			};
		AMonsterBase* Instance = GetWorld()->SpawnActor<AMonsterBase>(MonsterClass[MonsterData->Type], Loc, FRotator::ZeroRotator, param);
	}
}
