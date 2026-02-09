// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/BossMonster/BossMonster.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"

#include "Define/Enum.h"
#include "Data/MonsterData.h"


ABossMonster::ABossMonster()
{
	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTFinder(TEXT("/Script/AIModule.BehaviorTree'/Game/02-BP/Monster/AI/BT_BossMonster.BT_BossMonster'"));
	if (BTFinder.Succeeded())
		SetBehaviorTree(BTFinder.Object);

	static ConstructorHelpers::FObjectFinder<UBlackboardData> BBFinder(TEXT("/Script/AIModule.BlackboardData'/Game/02-BP/Monster/AI/BB_BossMonster.BB_BossMonster'"));
	if (BBFinder.Succeeded())
		SetBlackboardData(BBFinder.Object);
	
	ActionTotalWeights.Init(0.0f, static_cast<uint8>(EMonsterAttackType::END));
}

void ABossMonster::Init(const FMonsterInitParam& _param)
{
	Super::Init(_param);

	FMonsterData* MonsterData = GetData();
	for (const FMonsterAction& Action : MonsterData->AttackActions)
		ActionTotalWeights[static_cast<uint8>(Action.Type)] += Action.Weight;
}

void ABossMonster::Attack(FMonsterAttackParam* _param)
{
	GEngine->AddOnScreenDebugMessage(3, 3.0f, FColor::Blue, TEXT("Boss Try Attack"));

	if (nullptr == _param)
	{
		GEngine->AddOnScreenDebugMessage(3, 3.0f, FColor::Blue, TEXT("Returned Try Attack : _param is null"));
		return;
	}

	// 가중치에 따른 선별
	FMonsterData* MonsterData = GetData();
	if (nullptr == MonsterData)
	{
		GEngine->AddOnScreenDebugMessage(3, 3.0f, FColor::Blue, TEXT("Returned Try Attack : Data is null"));
		return;
	}
	
	float RandomValue = FMath::FRandRange(0.0f, ActionTotalWeights[static_cast<uint8>(_param->Type)]);
	float Sum = 0.0f;

	for (uint8 i = 0; i < MonsterData->AttackActions.Num(); ++i)
	{
		if (MonsterData->AttackActions[i].Type != _param->Type)
			continue;

		Sum += MonsterData->AttackActions[i].Weight;
		if (RandomValue <= Sum)
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
