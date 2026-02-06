// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/MeleeMonster/MeleeMonster.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "Components/WidgetComponent.h"
#include "Kismet/KismetSystemLibrary.h"

#include "Component/StatComponent.h"
#include "Data/MonsterData.h"

AMeleeMonster::AMeleeMonster()
{
	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTFinder(TEXT("/Script/AIModule.BehaviorTree'/Game/02-BP/Monster/AI/BT_MeleeMonster.BT_MeleeMonster'"));
	if (BTFinder.Succeeded())
		SetBehaviorTree(BTFinder.Object);

	static ConstructorHelpers::FObjectFinder<UBlackboardData> BBFinder(TEXT("/Script/AIModule.BlackboardData'/Game/02-BP/Monster/AI/BB_MeleeMonster.BB_MeleeMonster'"));
	if (BBFinder.Succeeded())
		SetBlackboardData(BBFinder.Object);
}

void AMeleeMonster::HandleAttackNotify(uint8 _opt)
{
	Super::HandleAttackNotify(_opt);

	// 히트 판정
	FVector Start = GetActorLocation();
	FVector End = Start + GetActorForwardVector() * GetData()->AttackRange;

	TArray<FHitResult> HitResults;
	bool IsHit = UKismetSystemLibrary::BoxTraceMulti(
		this,
		Start, End,
		FVector(50.0f, 50.0f, 50.0f), GetActorRotation(),
		UEngineTypes::ConvertToTraceType(ECC_GameTraceChannel3),
		false, { this },
		EDrawDebugTrace::None,
		HitResults,
		true
	);

	if (IsHit == false)
		return;

	for (FHitResult& Hit : HitResults)
	{
		IHitable* Hitable = Cast<IHitable>(Hit.GetActor());

		if (Hitable)
		{
			FHitInfo HitInfo;
			HitInfo.Damage = GetStatComp()->GetStat(ECharacterStatType::ATTACK);
			HitInfo.Attacker = this;
			HitInfo.HitResult = &Hit;

			Hitable->HitBy(HitInfo);
		}
	}
}

void AMeleeMonster::HitBy(const FHitInfo& _hitInfo)
{
	Super::HitBy(_hitInfo);
	KnockBack(_hitInfo);
}
