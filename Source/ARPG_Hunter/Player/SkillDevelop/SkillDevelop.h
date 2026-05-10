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
	// 스킬 포인트
	uint16 SkillPoint{ 0 };
	uint16 UsingSkillPoint{ 0 };
	// 스킬 육성 정보
	TMap<uint8, TMap<uint8, int8>> SkillSelectMap; // SkillTree - [NodeIdx - UpgradeIdx]

public:
	void Init();

	void AddSkillPoint(uint16 _point) { SkillPoint += _point; }
	bool TryUseSkillPoint(uint16 _point);

	uint16 GetSkillPoint() const { return SkillPoint; }
	uint16 GetUsingSkillPoint() const { return UsingSkillPoint; }
	uint16 GetUsableSkillPoint() const { return SkillPoint - UsingSkillPoint; }

	void AddSkill(uint8 _skillTree, uint8 _nodeIdx, uint8 _upgradeIdx);
	int8 GetSpecificSkillUpgrade(uint8 _skillTree, uint8 _nodeIdx) const;

	const TMap<uint8, TMap<uint8, int8>>& GetSkillSelect() const { return SkillSelectMap; }
	const TMap<uint8, TMap<uint8, int8>>* GetSkillSelectPtr() const { return &SkillSelectMap; }

	void LoadSkillPoints(uint16 _skillPoint, uint16 _usingSkillPoint) 
	{
		SkillPoint = _skillPoint;
		UsingSkillPoint = _usingSkillPoint;
	}

	void TestSetting();
};

