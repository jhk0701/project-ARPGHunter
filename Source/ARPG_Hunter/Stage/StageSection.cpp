// Fill out your copyright notice in the Description page of Project Settings.


#include "Stage/StageSection.h"
#include "Components/BoxComponent.h"

#include "Define/Debug.h"
#include "Core/GameMode/Combat/CombatGameMode.h"


// Sets default values
AStageSection::AStageSection()
{
	PrimaryActorTick.bCanEverTick = false;

	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("SectionArea"));
	SetRootComponent(BoxComp);

	BoxComp->SetCollisionProfileName(FName(TEXT("OverlapPlayer")));
}

void AStageSection::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (BoxComp)
		BoxComp->OnComponentBeginOverlap.AddDynamic(this, &AStageSection::OnBeginOverlap);
}

// Called when the game starts or when spawned
void AStageSection::BeginPlay()
{
	Super::BeginPlay();
	State = EState::READY;
	
	GetAttachedActors(SpawnPoints);
	
	if (SpawnPoints.Num() == 0)
		SpawnPoints.Add(this); // 스폰 포인트를 패치하지 않았다면 이 액터의 위치를 사용
}

void AStageSection::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (State > EState::READY)
		return;

	BeginSection();
}

void AStageSection::BeginSection()
{
	State = EState::IN_PROGRESS;
	SpawnMonster();
}

void AStageSection::EndSection()
{
	State = EState::CLEARED;

	TObjectPtr<ACombatGameMode> GameMode = GetWorld()->GetAuthGameMode<ACombatGameMode>();
	GameMode->StageEvent[EStageEvent::HUNT].Remove(EventHandle);

	FStageEventContext Context;
	Context.SectionIndex = Index;
	GameMode->PublishEvent(EStageEvent::SECTION_CLEAR, Context);
}

void AStageSection::SpawnMonster()
{
	// 게임모드에게 몬스터 스폰 요청
	TObjectPtr<ACombatGameMode> GameMode = GetWorld()->GetAuthGameMode<ACombatGameMode>();
	if (nullptr == GameMode)
	{
		State = EState::CLEARED;
		return;
	}

	SpawnedCount = GameMode->SpawnMonsterOnSection(Index, SpawnPoints);

	if (SpawnedCount > 0)
		EventHandle = GameMode->StageEvent[EStageEvent::HUNT].AddUObject(this, &AStageSection::OnMonsterDead);
	else
		State = EState::CLEARED;
}

void AStageSection::OnMonsterDead(const FStageEventContext& _context)
{
	if (_context.SectionIndex != Index)
		return;
	
	check(SpawnedCount > 0);
	SpawnedCount--;

	if (SpawnedCount == 0)
		EndSection();
}