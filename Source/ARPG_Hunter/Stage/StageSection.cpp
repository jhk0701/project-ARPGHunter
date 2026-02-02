// Fill out your copyright notice in the Description page of Project Settings.


#include "Stage/StageSection.h"
#include "Components/BoxComponent.h"

#include "GameMode/CombatGameMode.h"


// Sets default values
AStageSection::AStageSection()
{
	PrimaryActorTick.bCanEverTick = false;

	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("SectionArea"));
	SetRootComponent(BoxComp);
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

	State = EState::IN_PROGRESS;
	
	// 할당된 인덱스에 대한 스테이지 정보 받아오기
	// 클라이언트라면 null일 것
	ACombatGameMode* GameMode = Cast<ACombatGameMode>(GetWorld()->GetAuthGameMode());
	if (nullptr == GameMode)
	{
		State = EState::CLEARED;
		return;
	}

	GameMode->SpawnMonsterOnSection(Index, GetActorLocation(), BoxComp->GetScaledBoxExtent());
}