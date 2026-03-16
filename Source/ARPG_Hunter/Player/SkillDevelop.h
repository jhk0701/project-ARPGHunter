// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SkillDevelop.generated.h"

enum class EWeaponType : uint8;

/**
 * 
 */
UCLASS()
class ARPG_HUNTER_API USkillDevelop : public UObject
{
	GENERATED_BODY()

public:
	USkillDevelop();

private:
	uint16 SkillPoint{ 0 };

	// 무기 타입
	EWeaponType WeaponType;
	// 스킬 육성 정보
	TMap<uint8, TMap<uint8, int8>> SkillSelectMap; // SkillTree - [NodeIdx - UpgradeIdx]

public:
	void Init();

	void AddPoint(uint16 _point);
	void SubPoint(uint16 _point);
	uint16 GetSkillPoint() const { return SkillPoint; }

	void AddSkill(uint8 _skillTree, uint8 _nodeIdx, uint8 _upgradeIdx);
	const TMap<uint8, TMap<uint8, int8>>& GetSkillSelect() const { return SkillSelectMap; }
	const TMap<uint8, TMap<uint8, int8>>* GetSkillSelectPtr() const { return &SkillSelectMap; }

	void TestSetting();
};

