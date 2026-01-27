// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Effect.generated.h"

class UStatComponent;

UENUM()
enum class EEffectTargetStat
{
	STAT_ATTACK					UMETA(DisplayName = "Attack"),
	STAT_DEFENSE				UMETA(DisplayName = "Defense"),
	STAT_CRITICAL_PER			UMETA(DisplayName = "Critical Percent"),
	STAT_CRITICAL_DMG			UMETA(DisplayName = "Critical Damage"),

	RESOURCE_HEALTH = 100		UMETA(DisplayName = "Health"),
	RESOURCE_STAMINA			UMETA(DisplayName = "Stamina"),
	RESOURCE_SKILL				UMETA(DisplayName = "Skill"),
	RESOURCE_STAGGER			UMETA(DisplayName = "Stagger"),
};

struct FEffectParam
{
	EEffectTargetStat TargetStat;
	uint32 Value;
	float Duration;
};

/**
 * 
 */
UCLASS()
class ARPG_HUNTER_API UEffect : public UObject
{
	GENERATED_BODY()

private:
	TWeakObjectPtr<UStatComponent> TargetComp; // 효과 대상 : 약참조 소유
	FEffectParam* Param;

protected:
	TWeakObjectPtr<UStatComponent> GetTarget() { return TargetComp; }

public:
	virtual void Activate(TWeakObjectPtr<UStatComponent> _target, FEffectParam* _param) 
	{
		TargetComp = _target;
		Param = _param;
	};
	virtual void Deactivate() {};

	float GetDuration() { return Param->Duration; }
};
