// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Effect/Effect.h"
#include "TermEffect.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class ARPG_HUNTER_API UTermEffect : public UEffect
{
	GENERATED_BODY()

public:
	virtual void Activate(UStatComponent* _target, FEffectContext* _context) override;
	virtual void Deactivate() override;
};

UCLASS()
class ARPG_HUNTER_API UAddAttack : public UTermEffect 
{
	GENERATED_BODY()

public:
	void Activate(UStatComponent* _target, FEffectContext* _context) override;
	void Deactivate() override;
};

UCLASS()
class ARPG_HUNTER_API UAddAttackPercent : public UTermEffect
{
	GENERATED_BODY()

public:
	void Activate(UStatComponent* _target, FEffectContext* _context) override;
	void Deactivate() override;
};

UCLASS()
class ARPG_HUNTER_API UAddDefense : public UTermEffect
{
	GENERATED_BODY()

public:
	void Activate(UStatComponent* _target, FEffectContext* _context) override;
	void Deactivate() override;
};

UCLASS()
class ARPG_HUNTER_API UAddCriticalPercent : public UTermEffect
{
	GENERATED_BODY()

public:
	void Activate(UStatComponent* _target, FEffectContext* _context) override;
	void Deactivate() override;
};

UCLASS()
class ARPG_HUNTER_API UAddCriticalDamagePercent : public UTermEffect
{
	GENERATED_BODY()

public:
	void Activate(UStatComponent* _target, FEffectContext* _context) override;
	void Deactivate() override;
};
