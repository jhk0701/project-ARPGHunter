// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Monster/MonsterBase.h"
#include "BossMonster.generated.h"

enum class EMonsterAttackType : uint8;
enum class EGimicType : uint8;
struct FAttackDetail;

/**
 * 
 */
UCLASS()
class ARPG_HUNTER_API ABossMonster : public AMonsterBase
{
	GENERATED_BODY()

	enum EState : uint8 
	{
		NORMAL,
		IN_GIMIC,
		STAGGER
	};

private:
	EState CurState;
	EGimicType CurGimic;
	uint8 GimicValue;
	TArray<float> ActionTotalWeights;

	// Player HUD에 보스 체력바 출력
	// 데미지 폰트 위치 범위

	void MeleeAttack(const FAttackDetail& _detail);
	void RangedAttack(const FAttackDetail& _detail);

	void StopGimic();

public:
	ABossMonster();

	void Init(const FMonsterInitParam& _param) override;
	float Attack(FMonsterAttackParam* _param) override;
	void HandleAttackNotify(uint8 _opt) override;
	void HitBy(const FHitInfo& _hitInfo) override;

	void StartGimic(EGimicType _type);
	void ProceedGimic();
	void CompleteGimic();
};
