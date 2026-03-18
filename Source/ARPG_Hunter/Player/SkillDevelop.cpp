// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/SkillDevelop.h"

USkillDevelop::USkillDevelop()
{
}

void USkillDevelop::Init()
{
}

bool USkillDevelop::TryUseSkillPoint(uint16 _point)
{
	if (UsingSkillPoint + _point > SkillPoint)
		return false;

	UsingSkillPoint += _point;
	return true;
}

void USkillDevelop::AddSkill(uint8 _skillTree, uint8 _nodeIdx, uint8 _upgradeIdx)
{
	TMap<uint8, int8>& Selections = SkillSelectMap.FindOrAdd(_skillTree);
 	int8& Upgrade = Selections.FindOrAdd(_nodeIdx);
	Upgrade = _upgradeIdx;
}

int8 USkillDevelop::GetSpecificSkillUpgrade(uint8 _skillTree, uint8 _nodeIdx) const
{
	const TMap<uint8, int8>* Skill = SkillSelectMap.Find(_skillTree);
	if (nullptr == Skill)
		return -1;

	const int8* Upgrade = Skill->Find(_nodeIdx);
	if (nullptr == Upgrade)
		return -1;

	return *Upgrade;
}

void USkillDevelop::TestSetting()
{
	AddSkill(9, 0, 0);		// 9 - 0 - 0 Skill Charge
	AddSkill(10, 0, 0);		// 10 - 0 - 0 Dodge Slash 1
	// AddSkill(10, 2, 0);		// 10 - 2 - 0 Dodge Slash 2
	AddSkill(8, 2, 0);		// 8 - 2 - 0 Smash 4 - 2
	AddSkill(6, 3, 0);		// 6 - 3 - 0 Smash 3 - 2
	AddSkill(5, 1, 0);		// 5 - 1 - 0 Smash 2 - 2

	// Deep Wound
	AddSkill(4, 2, 0);
	AddSkill(5, 2, 0);
	AddSkill(6, 2, 0);
	AddSkill(6, 4, 0);
	AddSkill(8, 1, 0);
	
	// Wound Burst
	AddSkill(8, 4, 0);
	AddSkill(9, 2, 0);
}
