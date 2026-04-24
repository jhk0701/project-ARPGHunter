// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/GameMode/CombatGameMode.h"
#include "NavigationSystem.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/PlayerStart.h"
#include "EngineUtils.h"

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


uint32 ACombatGameMode::CalculateAttack(uint32 _baseVal, uint32 _AddictivePercent)
{
	return _baseVal * _AddictivePercent * 0.01f;
}

bool ACombatGameMode::CalculateCritical(uint32 _critPer, uint32 _critDmg, uint32& _outDamage)
{
	uint32 Critical = FMath::Rand() % 100;

	bool bIsCritical = Critical <= _critPer;
	if (bIsCritical)
		_outDamage *= (1.0f + _critDmg * 0.01f);

	return bIsCritical;
}

uint32 ACombatGameMode::CalculateDefense(uint32 _damage, uint32 _defense)
{
	uint32 Defense = _defense / 3;

	if (_damage >= Defense)
		_damage -= Defense;
	else
		_damage = 0;

	return _damage;
}

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

	for (TActorIterator<APlayerStart> ItPlayerStart(GetWorld()); ItPlayerStart; ++ItPlayerStart)
	{
		TObjectPtr<APlayerStart> PlayerStart = *ItPlayerStart;
		MapPlayerStart.Add(PlayerStart->PlayerStartTag, PlayerStart);
	}
}

void ACombatGameMode::BeginPlay()
{
	Super::BeginPlay();

	// GameState 초기화
	if (ACombatGameState* CombatGameState = GetGameState<ACombatGameState>()) 
	{
		// TODO: 멀티 플레이 시, 현재 플레이어들의 인원수 전달
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

		CombatGameState->Init(1, StageData->Sections);
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
		TMap<EMonsterType, uint8> CurSectionCountPerType;

		for (const FMonsterSpawn& Spawn : Section.Spawn)
		{
			EMonsterType Type = DataManager->GetMonsterData(Spawn.MonsterID)->Config->Type;
			if (nullptr == MaxCountPerType.Find(Type))
				MaxCountPerType.Add(Type, 0);
			if (nullptr == CurSectionCountPerType.Find(Type))
				CurSectionCountPerType.Add(Type, 0);

			CurSectionCountPerType[Type] += Spawn.Count;

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

		for (uint8 i = 0; i < static_cast<uint8>(EMonsterType::END); ++i)
		{
			EMonsterType Type = static_cast<EMonsterType>(i);
			if (CurSectionCountPerType.Find(Type) != nullptr && MaxCountPerType.Find(Type))
				MaxCountPerType[Type] = FMath::Max(CurSectionCountPerType[Type], MaxCountPerType[Type]);
		}
	}

	// 오브젝트 풀링 등록
	for (const TPair<EMonsterType, uint8>& pair : MaxCountPerType)
	{
		EMonsterType Type = pair.Key;
		TFunction<TObjectPtr<AActor>()> CreateFunc = 
			[this, Type]()
			{
				// 몬스터 액터 생성 람다식
				FActorSpawnParameters SpawnParam;
				SpawnParam.Owner = this;
				SpawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

				TObjectPtr<AMonsterBase> Inst = GetWorld()->SpawnActor<AMonsterBase>(MonsterClass[Type], SpawnParam);
				check(Inst != nullptr);

				// 몬스터 사망 시, 오브젝트 풀로 복귀하도록 이벤트에 바인딩
				Inst->OnMonsterDead.BindUObject(this, &ACombatGameMode::ReleaseMonster);

				return Inst;
			};

		ObjectPool->Register(
			MonsterClass[pair.Key],
			CreateFunc,
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

uint8 ACombatGameMode::SpawnMonsterOnSection(uint8 _sectionID, const TArray<AActor*>& _points)
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
			TObjectPtr<AActor> Inst = ObjectPool->Get(MonsterClass[MonsterData->Config->Type]);
			TObjectPtr<AMonsterBase> Instance = Cast<AMonsterBase>(Inst);

			FNavLocation Loc;
			FVector RandPos = _points[FMath::Rand() % _points.Num()]->GetActorLocation(); // UKismetMathLibrary::RandomPointInBoundingBox(_point, _areaSize);
			NavSys->GetRandomReachablePointInRadius(RandPos, 0.0f, Loc);

			bool bIsValid = false;
			for (int j = 0; j < 20; ++j) // 재시도 횟수 20번으로 고정
			{
				bIsValid = NavSys->ProjectPointToNavigation(Loc.Location, Loc, FVector(250.0f));
				if (bIsValid)
					break;
				else
					NavSys->GetRandomReachablePointInRadius(RandPos, 300.0f, Loc);
			}
						
			FRotator Rot(0, FMath::Rand() % 360, 0);

			FMonsterInitParam InitParam
			{
				Spawn.MonsterID,
				Spawn.Level,
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
	PlayerManager->AddExp(StageData->RewardExp);

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

AActor* ACombatGameMode::ChoosePlayerStart_Implementation(AController* _player)
{
	if (MapPlayerStart.Find(StageData->StartPoint) != nullptr)
		return MapPlayerStart[StageData->StartPoint];

	return Super::ChoosePlayerStart_Implementation(_player);
}

TObjectPtr<UCutSceneAssetData> ACombatGameMode::GetCutSceneAsset(uint8 _idx) const
{
	if(nullptr == StageData || StageData->CutSceneAssets.Num() <= _idx)
		return nullptr;

	return StageData->CutSceneAssets[_idx];
}