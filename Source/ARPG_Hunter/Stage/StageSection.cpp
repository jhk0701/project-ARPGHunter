// Fill out your copyright notice in the Description page of Project Settings.


#include "Stage/StageSection.h"
#include "Components/BoxComponent.h"

#include "GameMode/CombatGameMode.h"
#include "GameMode/GameState/CombatGameState.h"

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

	// 게임모드에게 몬스터 스폰 요청
	ACombatGameMode* GameMode = Cast<ACombatGameMode>(GetWorld()->GetAuthGameMode());
	if (nullptr == GameMode)
	{
		State = EState::CLEARED;
		return;
	}

	SpawnedCount = GameMode->SpawnMonsterOnSection(Index, GetActorLocation(), BoxComp->GetScaledBoxExtent());
	EventHandle = GameMode->GetGameState<ACombatGameState>()->StageEventBus[EStageEvent::HUNT].AddUObject(this, &AStageSection::OnMonsterDead);
}

void AStageSection::EndSection()
{
	State = EState::CLEARED;

	AGameModeBase* GameMode = GetWorld()->GetAuthGameMode();
	GameMode->GetGameState<ACombatGameState>()->StageEventBus[EStageEvent::HUNT].Remove(EventHandle);
}

void AStageSection::OnMonsterDead(const FStageEventContext& _context)
{
	if (_context.SectionIndex != Index)
		return;
	
	ensure(SpawnedCount > 0);

	SpawnedCount--;

	if (SpawnedCount == 0)
		EndSection();
}