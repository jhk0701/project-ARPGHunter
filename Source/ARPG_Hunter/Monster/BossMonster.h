// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Monster/MonsterBase.h"
#include "BossMonster.generated.h"

enum class EMonsterAttackType : uint8;
enum class EGimicType : uint8;

/**
 * 
 */
UCLASS()
class ARPG_HUNTER_API ABossMonster : public AMonsterBase
{
	GENERATED_BODY()
private:
	TArray<float> ActionTotalWeights;

	// Player HUD에 보스 체력바 출력
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UUserWidget> StatusBarClass;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UUserWidget> StatusBar;

protected:
	void BeginPlay() override;
	void OnDead() override;

public:
	ABossMonster();

	void Init(const FMonsterInitParam& _param) override;
	float Attack(EMonsterAttackType _type) override;
	void HitBy(const FHitInfo& _hitInfo) override;

	bool CanUseSkill();
	void StartGimic(EGimicType _type, uint16 _gimicValue);
};
