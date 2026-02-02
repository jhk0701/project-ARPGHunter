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
#include "Subsystem/ObjectPool/ObjectPoolManager.h"

#include "Define/Debug.h"

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

	// 몬스터 액터 풀링
	UObjectPoolManager* ObjectPool = GetWorld()->GetSubsystem<UObjectPoolManager>();
	UDataManager* DataManager = GetGameInstance()->GetSubsystem<UDataManager>();
	if (ObjectPool && DataManager)
	{
		// 특정 구역에서 요구하는 최대 개수 구하기
		TMap<EMonsterType, uint8> MaxCountPerType;
		for (const FSection& Section : StageData->Sections)
		{
			for (const FMonsterSpawn& Spawn : Section.Spawn)
			{
				EMonsterType Type = DataManager->GetMonsterData(Spawn.MonsterID)->Type;
				if(nullptr == MaxCountPerType.Find(Type))
					MaxCountPerType.Add(Type, 0);

				MaxCountPerType[Type] = FMath::Max(Spawn.Count, MaxCountPerType[Type]);
			}
		}

		// 오브젝트 풀링 등록
		for (const TPair<EMonsterType, uint8>& pair : MaxCountPerType)
		{
			EMonsterType Type = pair.Key;

			ObjectPool->Register(
				MonsterClass[pair.Key], 
				[this, Type]()
				{
					return GetWorld()->SpawnActor<AMonsterBase>(MonsterClass[Type]);
				}, 
				pair.Value);
		}
	}
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
	UObjectPoolManager* ObjectPool = GetWorld()->GetSubsystem<UObjectPoolManager>();
	UDataManager* DataManager = GetGameInstance()->GetSubsystem<UDataManager>();
	FMonsterData* MonsterData = DataManager->GetMonsterData(_spawnData.MonsterID);

	UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
	if (nullptr == NavSys)
		return;

	for (int i = 0; i < _spawnData.Count; ++i)
	{
		FNavLocation Loc;
		NavSys->GetRandomReachablePointInRadius(_point, 500.0f, Loc);
		FRotator Rot(0, FMath::Rand() % 360, 0);

		AActor* Inst = ObjectPool->Get(MonsterClass[MonsterData->Type]);

		if (Inst == nullptr)
		{
			UE_LOG(LogARPG, Error, TEXT("Object Pool Failed"));
		}

		AMonsterBase* Instance = Cast<AMonsterBase>(Inst); 
		Instance->Init(_spawnData.MonsterID, Loc, Rot);
	}
}