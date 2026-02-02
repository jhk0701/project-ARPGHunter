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

	StartSection();
}

void AStageSection::StartSection()
{
	State = EState::IN_PROGRESS;

	// 게임모드에게 몬스터 스폰 요청
	ACombatGameMode* GameMode = Cast<ACombatGameMode>(GetWorld()->GetAuthGameMode());
	if (nullptr == GameMode)
	{
		State = EState::CLEARED;
		return;
	}

	GameMode->SpawnMonsterOnSection(Index, GetActorLocation(), BoxComp->GetScaledBoxExtent());
	EventHandle = GameMode->GetGameState<ACombatGameState>()->GetEvent(EStageEvent::HUNT).AddUObject(this, &AStageSection::OnMonsterDead);
}

void AStageSection::OnMonsterDead(const FStageEventContext& _context)
{

}