// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/GameMode/CombatGameMode.h"
#include "NavigationSystem.h"
#include "Kismet/KismetMathLibrary.h"

#include "Define/Enum.h"
#include "Core/ARPGGameInstance.h"
#include "Core/Subsystem/PlayerManager.h"
#include "Core/Subsystem/DataManager.h"
#include "Core/WorldSubsystem/ObjectPoolManager.h"
#include "Core/GameState/CombatGameState.h"
#include "Controller/PlayerCombatController.h"
#include "Data/StageData.h"
#include "Data/MonsterData.h"
#include "Data/MonsterConfig.h"
#include "Data/Action.h"
#include "Monster/MonsterBase.h"
#include "SubObject/SubObject.h"
#include "UI/CombatHUD.h"
#include "Define/Debug.h"

ACombatGameMode::ACombatGameMode()
{
	PlayerControllerClass = APlayerCombatController::StaticClass();
	GameStateClass = ACombatGameState::StaticClass();
	HUDClass = ACombatHUD::StaticClass();

	if (nullptr == MonsterClass.Find(EMonsterType::REGULAR))
		MonsterClass.Add(EMonsterType::REGULAR);
	if (nullptr == MonsterClass.Find(EMonsterType::BOSS))
		MonsterClass.Add(EMonsterType::BOSS);

	static ConstructorHelpers::FClassFinder<AMonsterBase> RegularMonFinder(TEXT("/Game/02-BP/Monster/BP_RegularMonster.BP_RegularMonster_C"));
	if (RegularMonFinder.Succeeded())
		MonsterClass[EMonsterType::REGULAR] = RegularMonFinder.Class;
	static ConstructorHelpers::FClassFinder<AMonsterBase> BossMonFinder(TEXT("/Game/02-BP/Monster/BP_BossMonster.BP_BossMonster_C"));
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
	if (nullptr == GI)
		return;

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

		CombatGameState->OnSectionCleared.BindLambda(
			[this](bool _bIsCleared)
			{
				if (_bIsCleared)
					GameClear();
			}
		);
		CombatGameState->OnPlayerDead.BindLambda(
			[this](uint8 _cnt) 
			{
				if (_cnt == 0)
					GameFail();
			}
		);
	}

	// 오브젝트 풀링
	RegisterObjectPool();
}

void ACombatGameMode::SetStageData(const FName& _stageID)
{
	UARPGGameInstance* GI = Cast<UARPGGameInstance>(GetGameInstance());
	if (nullptr == GI)
		return;
	StageData = GI->GetSubsystem<UDataManager>()->GetStageData(_stageID);
}

void ACombatGameMode::RegisterObjectPool()
{
	UObjectPoolManager* ObjectPool = GetWorld()->GetSubsystem<UObjectPoolManager>();
	UDataManager* DataManager = GetGameInstance()->GetSubsystem<UDataManager>();

	// 특정 구역에서 요구하는 최대 개수 구하기
	TMap<EMonsterType, uint8> MaxCountPerType;
	TMap<UClass*, uint8> MaxCountSubObject;

	for (const FSection& Section : StageData->Sections)
	{
		for (const FMonsterSpawn& Spawn : Section.Spawn)
		{
			EMonsterType Type = DataManager->GetMonsterData(Spawn.MonsterID)->Config->Type;
			if (nullptr == MaxCountPerType.Find(Type))
				MaxCountPerType.Add(Type, 0);

			MaxCountPerType[Type] = FMath::Max(Spawn.Count, MaxCountPerType[Type]);
			
			FMonsterData* MonsterData = DataManager->GetMonsterData(Spawn.MonsterID);
			
			for (const FMonsterAction& ActionData : MonsterData->Config->AttackActions)
			{
				if (nullptr == ActionData.Action->SubObjectClass)
					continue; 
				
				uint8* Cnt = MaxCountSubObject.Find(ActionData.Action->SubObjectClass);
				if (nullptr != Cnt)
				{
					*Cnt += 5;
					continue;
				}

				MaxCountSubObject.Add(ActionData.Action->SubObjectClass, 5);
			}
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

	for (const TPair<UClass*, uint8>& pair : MaxCountSubObject)
	{
		UClass* ClassToSpawn = pair.Key;

		ObjectPool->Register(
			pair.Key,
			[this, ClassToSpawn]()
			{
				FActorSpawnParameters SpawnParam;
				SpawnParam.Owner = this;

				ASubObject* Inst = GetWorld()->SpawnActor<ASubObject>(ClassToSpawn, SpawnParam);

				Inst->OnDisable.BindUObject(this, &ACombatGameMode::ReleaseSubObject);

				return Inst;
			},
			pair.Value
		);
	}
}

uint8 ACombatGameMode::SpawnMonsterOnSection(uint8 _sectionID, const FVector& _point, const FVector& _areaSize)
{
	if (StageData->Sections.Num() <= _sectionID)
		return 0;

	const FSection& SectionData = StageData->Sections[_sectionID];
	TObjectPtr<UObjectPoolManager> ObjectPool = GetWorld()->GetSubsystem<UObjectPoolManager>();
	TObjectPtr<UDataManager> DataManager = GetGameInstance()->GetSubsystem<UDataManager>();
	
	TObjectPtr<UNavigationSystemV1> NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
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

			TObjectPtr<AActor> Inst = ObjectPool->Get(MonsterClass[MonsterData->Config->Type]);
			TObjectPtr<AMonsterBase> Instance = Cast<AMonsterBase>(Inst);

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

void ACombatGameMode::ReleaseMonster(TObjectPtr<class AMonsterBase> _target)
{
	GetWorld()->GetSubsystem<UObjectPoolManager>()->Release(MonsterClass[_target->GetType()], _target);
}

void ACombatGameMode::ReleaseSubObject(TObjectPtr<AActor> _target)
{
	GetWorld()->GetSubsystem<UObjectPoolManager>()->Release(_target.GetClass(), _target);
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

	OnGameEnd.Broadcast(true, StageData);
}

void ACombatGameMode::GameFail()
{
	UE_LOG(LogARPG, Log, TEXT("Stage Failed"));

	OnGameEnd.Broadcast(false, nullptr);
}
