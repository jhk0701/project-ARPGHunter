// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/CombatGameMode.h"
#include "NavigationSystem.h"

#include "Controller/PlayerCombatController.h"
#include "UI/PlayerHUD.h"
#include "Define/Enum.h"
#include "Core/ARPGGameInstance.h"
#include "Subsystem/DataManager/DataManager.h"
#include "Data/StageData.h"
#include "Monster/MonsterBase.h"
#include "Data/MonsterData.h"

ACombatGameMode::ACombatGameMode()
{
	PlayerControllerClass = APlayerCombatController::StaticClass();
	HUDClass = APlayerHUD::StaticClass();

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


void ACombatGameMode::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	// 게임 인스턴스에 저장된 스테이지 ID 데이터 읽어오기
	UARPGGameInstance* GI = Cast<UARPGGameInstance>(GetGameInstance());
	check(nullptr != GI);

	if (GI->GetStageID().IsNone())
	{
		StageData = GI->GetSubsystem<UDataManager>()->GetStageData(FName(TEXT("0")));
		return;
	}

	StageData = GI->GetSubsystem<UDataManager>()->GetStageData(GI->GetStageID());
}

void ACombatGameMode::BeginPlay()
{
	Super::BeginPlay();

	// 게임 진행 준비
	bSectionIsCleared.SetNum(StageData->Sections.Num());
	for (int i = 0; i < StageData->Sections.Num(); ++i)
		bSectionIsCleared[i] = false;
}

const FSection& ACombatGameMode::GetSection(uint8 _idx) const
{
	check(StageData);
	return StageData->Sections[_idx];
}

void ACombatGameMode::SpawnMonsterOnSection(uint8 _sectionID, const FVector& _point)
{
	const FSection& SectionData = GetSection(_sectionID);
	for (const FMonsterSpawn& Spawn : SectionData.Spawn)
		SpawnMonster(Spawn, _point);
}

void ACombatGameMode::SpawnMonster(const FMonsterSpawn& _spawnData, const FVector& _point)
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

	UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
	if (nullptr == NavSys)
		return;

	for (int i = 0; i < _spawnData.Count; ++i)
	{
		FNavLocation Loc;
		NavSys->GetRandomReachablePointInRadius(_point, 500.0f, Loc);

		FRotator Rot(0, FMath::Rand() % 360, 0);
		GetWorld()->SpawnActor<AMonsterBase>(MonsterClass[MonsterData->Type], Loc.Location, Rot, Param);
	}
}