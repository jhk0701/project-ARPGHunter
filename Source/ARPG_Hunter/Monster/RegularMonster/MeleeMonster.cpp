// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/RegularMonster/MeleeMonster.h"
#include "Components/WidgetComponent.h"

#include "Component/StatComponent.h"
#include "Data/MonsterData.h"
#include "Data/AttackConfig.h"

void AMeleeMonster::HandleAttackNotify(uint8 _opt)
{
	Super::HandleAttackNotify(_opt);

	FAttackParam Param;
	Param.Subject = this;
	Param.Channel = ECC_GameTraceChannel3;

	const FAttackDetail& CurDetail = GetCurAction()->AttackDetails[_opt];
	Param.Size = CurDetail.Size;
	Param.Range = CurDetail.Range;
	Param.DetailType = CurDetail.Type;
	Param.OnHitAction = 
		[this](FHitResult& _hitResult) 
		{
			IHitable* Hitable = Cast<IHitable>(_hitResult.GetActor());

			if (Hitable)
			{
				FHitInfo HitInfo;
				HitInfo.Damage = GetStatComp()->GetStat(ECharacterStatType::ATTACK);
				HitInfo.Attacker = this;
				HitInfo.HitResult = &_hitResult;

				Hitable->HitBy(HitInfo);
			}
		};

	UAttackConfig::Act(Param);
}
