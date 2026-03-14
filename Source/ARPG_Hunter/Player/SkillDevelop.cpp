// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/SkillDevelop.h"

USkillDevelop::USkillDevelop()
{
}

void USkillDevelop::Init()
{
}

void USkillDevelop::AddPoint(uint16 _point)
{
	SkillPoint += _point;
}

void USkillDevelop::SubPoint(uint16 _point)
{
	check(SkillPoint >= _point);
	SkillPoint -= _point;
}
