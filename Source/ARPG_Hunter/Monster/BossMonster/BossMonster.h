// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Monster/MonsterBase.h"
#include "Interface/GimicHandler.h"
#include "BossMonster.generated.h"

enum class EMonsterAttackType : uint8;
struct FAttackDetail;
class UGimicAction;

/**
 * 
 */
UCLASS()
class ARPG_HUNTER_API ABossMonster : public AMonsterBase, public IGimicHandler
{
	GENERATED_BODY()
private:
	TArray<float> ActionTotalWeights;
	TObjectPtr<UGimicAction> CurGimic;

	// Player HUD에 보스 체력바 출력
	// 데미지 폰트 위치 범위
	void MeleeAttack(const FAttackDetail& _detail);
	void RangedAttack(const FAttackDetail& _detail);

protected:
	void OnDead() override;

public:
	ABossMonster();

	void Init(const FMonsterInitParam& _param) override;
	float Attack(FMonsterAttackParam* _param) override;
	void HandleAttackNotify(uint8 _opt) override;
	void HitBy(const FHitInfo& _hitInfo) override;

	void StartGimic(EGimicType _type) override;
	void ProceedGimic(float _deltaSecond) override;
	void CompleteGimic() override;
	void StopGimic(EGimicType _type) override;
};
