// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/MeleeMonster/MeleeMonster.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"

AMeleeMonster::AMeleeMonster()
{
	// 
	static ConstructorHelpers::FObjectFinder<UBehaviorTree> MeleeMonsterBTFinder(TEXT("/Script/AIModule.BehaviorTree'/Game/02-BP/Monster/AI/BT_MeleeMonster.BT_MeleeMonster'"));
	if (MeleeMonsterBTFinder.Succeeded())
		SetBehaviorTree(MeleeMonsterBTFinder.Object);

	static ConstructorHelpers::FObjectFinder<UBlackboardData> MeleeMonsterBBFinder(TEXT("/Script/AIModule.BlackboardData'/Game/02-BP/Monster/AI/BB_MeleeMonster.BB_MeleeMonster'"));
	if (MeleeMonsterBBFinder.Succeeded())
		SetBlackboardData(MeleeMonsterBBFinder.Object);
}
