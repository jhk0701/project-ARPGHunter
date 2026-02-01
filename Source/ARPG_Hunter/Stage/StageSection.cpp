// Fill out your copyright notice in the Description page of Project Settings.


#include "Stage/StageSection.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetMathLibrary.h"

#include "GameMode/CombatGameMode.h"
#include "Data/StageData.h"
#include "Define/Enum.h"
#include "Monster/MonsterBase.h"
#include "Subsystem/DataManager/DataManager.h"
#include "Data/MonsterData.h"

#include "Define/Debug.h"
#include "Player/PlayerCharacter.h"


// Sets default values
AStageSection::AStageSection()
{
	PrimaryActorTick.bCanEverTick = false;

	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawnArea"));
	SetRootComponent(BoxComp);

	BoxComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	if (nullptr == MonsterClass.Find(EMonsterType::MELEE))
		MonsterClass.Add(EMonsterType::MELEE);
	if (nullptr == MonsterClass.Find(EMonsterType::RANGED))
		MonsterClass.Add(EMonsterType::RANGED);
	if (nullptr == MonsterClass.Find(EMonsterType::BOSS))
		MonsterClass.Add(EMonsterType::BOSS);

	static ConstructorHelpers::FClassFinder<AMonsterBase> MeleeMonFinder(TEXT("/Game/02-BP/Monster/BP_MeleeMonster.BP_MeleeMonster_C"));
	if (MeleeMonFinder.Succeeded())
		MonsterClass[EMonsterType::MELEE] = MeleeMonFinder.Class;
	static ConstructorHelpers::FClassFinder<AMonsterBase> RangedMonFinder(TEXT("/Game/02-BP/Monster/BP_MeleeMonster.BP_MeleeMonster_C"));
	if (RangedMonFinder.Succeeded())
		MonsterClass[EMonsterType::RANGED] = RangedMonFinder.Class;
	static ConstructorHelpers::FClassFinder<AMonsterBase> BossMonFinder(TEXT("/Game/02-BP/Monster/BP_MeleeMonster.BP_MeleeMonster_C"));
	if (BossMonFinder.Succeeded())
		MonsterClass[EMonsterType::BOSS] = BossMonFinder.Class;
}

void AStageSection::PostInitializeComponents()
{
	Super::PostInitializeComponents();

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
	if (APlayerCharacter* Player = Cast<APlayerCharacter>(OtherActor))
	{
		UE_LOG(LogARPG, Display, TEXT("Player Overlap"));
	}
	else
		return;

	if (State > EState::READY)
		return;

	State = EState::IN_PROGRESS;
	
	// 할당된 인덱스에 대한 스테이지 정보 받아오기
	// 클라이언트라면 null일 것
	ACombatGameMode* GameMode = Cast<ACombatGameMode>(GetWorld()->GetAuthGameMode());
	if (nullptr == GameMode)
		return;

	const FSection& SectionData = GameMode->GetSection(Index);
	for (const FMonsterSpawn& Spawn : SectionData.Spawn)
		SpawnMonster(Spawn);
}

void AStageSection::SpawnMonster(const FMonsterSpawn& _spawnData)
{
	UDataManager* DataManager = GetGameInstance()->GetSubsystem<UDataManager>();
	FMonsterData* MonsterData = DataManager->GetMonsterData(_spawnData.MonsterID);

	// 생성 후 즉시 ID 부여
	FActorSpawnParameters Param;
	Param.CustomPreSpawnInitalization =
		[&_spawnData](AActor* _inst)
		{
			AMonsterBase* Inst = Cast<AMonsterBase>(_inst);
			Inst->SetID(_spawnData.MonsterID);
		};

	for (int i = 0; i < _spawnData.Count; ++i)
	{
		// 박스 크기 내에서 랜덤하게 생성
		FVector Loc = UKismetMathLibrary::RandomPointInBoundingBox(GetActorLocation(), BoxComp->GetScaledBoxExtent());
		FRotator Rot(0, FMath::Rand() % 360, 0);
		GetWorld()->SpawnActor<AMonsterBase>(MonsterClass[MonsterData->Type], Loc, Rot, Param);
	}
}