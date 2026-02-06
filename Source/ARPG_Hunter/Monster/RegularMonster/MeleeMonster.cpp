// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/RegularMonster/MeleeMonster.h"
#include "Components/WidgetComponent.h"
#include "Kismet/KismetSystemLibrary.h"

#include "Component/StatComponent.h"
#include "Data/MonsterData.h"


AMeleeMonster::AMeleeMonster()
{
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
