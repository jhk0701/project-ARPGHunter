// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SkillDevelop.generated.h"

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

public:
	void Init();

	void AddPoint(uint16 _point);
	void SubPoint(uint16 _point);
	uint16 GetSkillPoint() const { return SkillPoint; }
};
