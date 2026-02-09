// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Monster/MonsterBase.h"
#include "BossMonster.generated.h"

enum class EMonsterAttackType : uint8;

/**
 * 
 */
UCLASS()
class ARPG_HUNTER_API ABossMonster : public AMonsterBase
{
	GENERATED_BODY()

private:
	// Player HUD에 보스 체력바 출력
	// 데미지 폰트 위치 범위
	TArray<float> ActionTotalWeights;

public:
	ABossMonster();

	void Init(const FMonsterInitParam& _param) override;
	void Attack(FMonsterAttackParam* _param) override;

	void HandleAttackNotify(uint8 _opt) override;
};
