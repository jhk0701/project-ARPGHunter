// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/CombatGameMode.h"
#include "NavigationSystem.h"
#include "Kismet/KismetMathLibrary.h"

#include "Define/Enum.h"
#include "Core/ARPGGameInstance.h"
#include "Subsystem/PlayerManager/PlayerManager.h"
#include "Subsystem/DataManager/DataManager.h"
#include "Subsystem/ObjectPool/ObjectPoolManager.h"
#include "Data/StageData.h"
#include "Data/MonsterData.h"
#include "GameMode/GameState/CombatGameState.h"
#include "Controller/PlayerCombatController.h"
#include "UI/PlayerHUD.h"
#include "Monster/MonsterBase.h"

#include "Define/Debug.h"

ACombatGameMode::ACombatGameMode()
{
	PlayerControllerClass = APlayerCombatController::StaticClass();
	GameStateClass = ACombatGameState::StaticClass();
	HUDClass = APlayerHUD::StaticClass();

	if (nullptr == MonsterClass.Find(EMonsterType::MELEE))
		MonsterClass.Add(EMonsterType::MELEE);
	if (nullptr == MonsterClass.Find(EMonsterType::RANGED))
		MonsterClass.Add(EMonsterType::RANGED);
	if (nullptr == MonsterClass.Find(EMonsterType::BOSS))
		MonsterClass.Add(EMonsterType::BOSS);

	// TODO : 몬스터 다양화 때, 임시 경로 변경
	static ConstructorHelpers::FClassFinder<AMonsterBase> MeleeMonFinder(TEXT("/Game/02-BP/Monster/BP_MeleeMonster.BP_MeleeMonster_C"));
	if (MeleeMonFinder.Succeeded())
		MonsterClass[EMonsterType::MELEE] = MeleeMonFinder.Class;
	static ConstructorHelpers::FClassFinder<AMonsterBase> RangedMonFinder(TEXT("/Game/02-BP/Monster/BP_MeleeMonster.BP_MeleeMonster_C"));
	if (RangedMonFinder.Succeeded())
		MonsterClass[EMonsterType::RANGED] = RangedMonFinder.Class;
	static ConstructorHelpers::FClassFinder<AMonsterBase> BossMonFinder(TEXT("/Game/02-BP/Monster/BP_MeleeMonster.BP_MeleeMonster_C"));
	if (BossMonFinder.Succeeded())
		MonsterClass[EMonsterType::BOSS] = BossMonFinder.Class;

	// 스테이지 이벤트 버스 초기화
	for (uint8 i = 0; i < static_cast<uint8>(EStageEvent::END); ++i)
		StageEvent.Add(static_cast<EStageEvent>(i));
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

	// GameState 초기화
	if (ACombatGameState* CombatGameState = GetGameState<ACombatGameState>()) 
	{
		// TODO: 멀티 플레이 시, 현재 플레이어들의 인원수 전달
		CombatGameState->Init(1, StageData->Sections);
		CombatGameState->OnPlayerDead.BindLambda(
			[this](uint8 _cnt) 
			{
				if (_cnt == 0)
					GameFail();
			}
		);
		CombatGameState->OnSectionCleared.BindLambda(
			[this](bool _bIsCleared) 
			{
				if (_bIsCleared)
					GameClear();
			}
		);
	}

	// 몬스터 액터 풀링
	SetMonsterPool();
}

void ACombatGameMode::SetMonsterPool()
{
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
				if (nullptr == MaxCountPerType.Find(Type))
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
					// 몬스터 액터 생성 람다식
					FActorSpawnParameters SpawnParam;
					SpawnParam.Owner = this;
					AMonsterBase* Inst = GetWorld()->SpawnActor<AMonsterBase>(MonsterClass[Type], SpawnParam);

					// 몬스터 사망 시, 오브젝트 풀로 복귀하도록 이벤트에 바인딩
					Inst->OnMonsterDead.BindUObject(this, &ACombatGameMode::ReleaseMonster);

					return Inst;
				},
				pair.Value);
		}
	}
}

uint8 ACombatGameMode::SpawnMonsterOnSection(uint8 _sectionID, const FVector& _point, const FVector& _areaSize)
{
	if (StageData->Sections.Num() <= _sectionID)
		return 0;

	const FSection& SectionData = StageData->Sections[_sectionID];
	UObjectPoolManager* ObjectPool = GetWorld()->GetSubsystem<UObjectPoolManager>();
	UDataManager* DataManager = GetGameInstance()->GetSubsystem<UDataManager>();
	
	UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
	if (nullptr == NavSys)
		return 0;

	uint8 SpawnedCount = 0;

	for (const FMonsterSpawn& Spawn : SectionData.Spawn)
	{
		FMonsterData* MonsterData = DataManager->GetMonsterData(Spawn.MonsterID);

		for (int i = 0; i < Spawn.Count; ++i)
		{
			FNavLocation Loc;
			FVector RandBoxPos = UKismetMathLibrary::RandomPointInBoundingBox(_point, _areaSize);
			NavSys->GetRandomReachablePointInRadius(RandBoxPos, 100.0f, Loc);
			FRotator Rot(0, FMath::Rand() % 360, 0);

			AActor* Inst = ObjectPool->Get(MonsterClass[MonsterData->Type]);
			AMonsterBase* Instance = Cast<AMonsterBase>(Inst);

			FMonsterInitParam InitParam
			{
				Spawn.MonsterID,
				_sectionID,
				Loc.Location,
				Rot
			};
			Instance->Init(InitParam);

			SpawnedCount++;
		}
	}

	return SpawnedCount;
}

void ACombatGameMode::GameClear()
{
	UE_LOG(LogARPG, Log, TEXT("Stage Clear!"));

	// 보상 지급
	// 골드 재화 지급
	UPlayerManager* PlayerManager = GetGameInstance()->GetSubsystem<UPlayerManager>();
	PlayerManager->AddGold(StageData->RewardGold);

	// 아이템 지급
	for (const FRewardItem& Item : StageData->RewardItems)
		PlayerManager->AddItem(Item.ID, Item.Count);

	OnGameEnd.Broadcast(true);
}

void ACombatGameMode::GameFail()
{
	UE_LOG(LogARPG, Log, TEXT("Stage Failed"));

	OnGameEnd.Broadcast(false);
}

void ACombatGameMode::ReleaseMonster(TObjectPtr<class AMonsterBase> _target)
{
	GetWorld()->GetSubsystem<UObjectPoolManager>()->Release(MonsterClass[_target->GetType()], _target);
}