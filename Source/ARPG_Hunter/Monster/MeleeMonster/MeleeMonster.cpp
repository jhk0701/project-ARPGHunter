// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/MeleeMonster/MeleeMonster.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "Components/WidgetComponent.h"

AMeleeMonster::AMeleeMonster()
{
	static ConstructorHelpers::FObjectFinder<UBehaviorTree> MeleeMonsterBTFinder(TEXT("/Script/AIModule.BehaviorTree'/Game/02-BP/Monster/AI/BT_MeleeMonster.BT_MeleeMonster'"));
	if (MeleeMonsterBTFinder.Succeeded())
		SetBehaviorTree(MeleeMonsterBTFinder.Object);

	static ConstructorHelpers::FObjectFinder<UBlackboardData> MeleeMonsterBBFinder(TEXT("/Script/AIModule.BlackboardData'/Game/02-BP/Monster/AI/BB_MeleeMonster.BB_MeleeMonster'"));
	if (MeleeMonsterBBFinder.Succeeded())
		SetBlackboardData(MeleeMonsterBBFinder.Object);

	UWidgetComponent* Widget = GetWidgetComp();
	static ConstructorHelpers::FClassFinder<UUserWidget> StatusUIFinder(TEXT("/Game/06-UI/WBP_MonsterStatusBar.WBP_MonsterStatusBar_C"));
	if(StatusUIFinder.Succeeded() && Widget)
		Widget->SetWidgetClass(StatusUIFinder.Class);

}

void AMeleeMonster::HitBy(const FHitInfo& _hitInfo)
{
	Super::HitBy(_hitInfo);

	KnockBack(_hitInfo);
}
