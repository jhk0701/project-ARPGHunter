// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Effect/TermEffect/TermEffect.h"
#include "PeriodicalEffect.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class ARPG_HUNTER_API UPeriodicalEffect : public UTermEffect
{
	GENERATED_BODY()
private:
	FTimerHandle RepeatTimer;

public:
	virtual bool Activate() final;
	virtual void Deactivate() override;

	virtual void RepeatedActivate() {}; // 자식 클래스에서 구체적인 동작 위임
};

UCLASS()
class ARPG_HUNTER_API URepeatlyRecoverHealth : public UPeriodicalEffect
{
	GENERATED_BODY()
public:
	void RepeatedActivate() override;
};

UCLASS()
class ARPG_HUNTER_API URepeatlyRecoverStamina : public UPeriodicalEffect
{
	GENERATED_BODY()
public:
	void RepeatedActivate() override;
};

UCLASS()
class ARPG_HUNTER_API URepeatlyRecoverSkill : public UPeriodicalEffect
{
	GENERATED_BODY()
public:
	void RepeatedActivate() override;
};

UCLASS()
class ARPG_HUNTER_API URepeatlyDamageHealth : public UPeriodicalEffect
{
	GENERATED_BODY()
public:
	void RepeatedActivate() override;
};

UCLASS()
class ARPG_HUNTER_API URepeatlyDamageStamina : public UPeriodicalEffect
{
	GENERATED_BODY()
public:
	void RepeatedActivate() override;
};