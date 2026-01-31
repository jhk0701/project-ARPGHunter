// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/MeleeMonster/MeleeMonster.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "Components/WidgetComponent.h"

AMeleeMonster::AMeleeMonster()
{
	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTFinder(TEXT("/Script/AIModule.BehaviorTree'/Game/02-BP/Monster/AI/BT_MeleeMonster.BT_MeleeMonster'"));
	if (BTFinder.Succeeded())
		SetBehaviorTree(BTFinder.Object);

	static ConstructorHelpers::FObjectFinder<UBlackboardData> BBFinder(TEXT("/Script/AIModule.BlackboardData'/Game/02-BP/Monster/AI/BB_MeleeMonster.BB_MeleeMonster'"));
	if (BBFinder.Succeeded())
		SetBlackboardData(BBFinder.Object);
}

void AMeleeMonster::HitBy(const FHitInfo& _hitInfo)
{
	Super::HitBy(_hitInfo);

	KnockBack(_hitInfo);
}
