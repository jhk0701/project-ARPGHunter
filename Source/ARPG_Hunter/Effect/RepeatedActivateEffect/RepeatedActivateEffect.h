// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Effect/Effect.h"
#include "RepeatedActivateEffect.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class ARPG_HUNTER_API URepeatedActivateEffect : public UEffect
{
	GENERATED_BODY()
private:
	FTimerHandle RepeatTimer;

public:
	void Activate(UStatComponent* _target, FEffectParam* _param) final;
	virtual void Deactivate() override;

	virtual void RepeatedActivate() {}; // 자식 클래스에서 구체적인 동작 위임
};

UCLASS()
class ARPG_HUNTER_API URecoverHealth : public URepeatedActivateEffect
{
	GENERATED_BODY()
public:
	void RepeatedActivate() override;
};

UCLASS()
class ARPG_HUNTER_API URecoverStamina : public URepeatedActivateEffect
{
	GENERATED_BODY()
public:
	void RepeatedActivate() override;
};

UCLASS()
class ARPG_HUNTER_API URecoverSkill : public URepeatedActivateEffect
{
	GENERATED_BODY()
public:
	void RepeatedActivate() override;
};

UCLASS()
class ARPG_HUNTER_API UDamageHealth : public URepeatedActivateEffect
{
	GENERATED_BODY()
public:
	void RepeatedActivate() override;
};

UCLASS()
class ARPG_HUNTER_API UDamageStamina : public URepeatedActivateEffect
{
	GENERATED_BODY()
public:
	void RepeatedActivate() override;
};