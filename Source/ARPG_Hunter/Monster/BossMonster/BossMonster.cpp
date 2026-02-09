// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/BossMonster/BossMonster.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"

#include "Data/MonsterData.h"


ABossMonster::ABossMonster()
{
	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTFinder(TEXT("/Script/AIModule.BehaviorTree'/Game/02-BP/Monster/AI/BT_BossMonster.BT_BossMonster'"));
	if (BTFinder.Succeeded())
		SetBehaviorTree(BTFinder.Object);

	static ConstructorHelpers::FObjectFinder<UBlackboardData> BBFinder(TEXT("/Script/AIModule.BlackboardData'/Game/02-BP/Monster/AI/BB_BossMonster.BB_BossMonster'"));
	if (BBFinder.Succeeded())
		SetBlackboardData(BBFinder.Object);
}

void ABossMonster::Init(const FMonsterInitParam& _param)
{
	Super::Init(_param);

	FMonsterData* MonsterData = GetData();
	for (const FMonsterAction& Action : MonsterData->AttackActions)
		TotalWeight += Action.Weight;
}

void ABossMonster::Attack(FMonsterAttackParam* _param)
{
	// 패턴 : 일반 3
	// 특수 : 카운터, 일반 무력화
	// 가중치에 따른 선별
	FMonsterData* MonsterData = GetData();
	if (nullptr == MonsterData)
		return;
	
	float RandomValue = FMath::FRandRange(0.0f, TotalWeight);
	float Sum = 0.0f;

	for (uint8 i = 0; i < MonsterData->AttackActions.Num(); ++i)
	{
		Sum += MonsterData->AttackActions[i].Weight;
		if (Sum >= RandomValue)
		{
			SetCurAttackIdx(i);
			break;
		}
	}

	Super::Attack();
}

void ABossMonster::HandleAttackNotify(uint8 _opt)
{
}
