// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Component/StatComponent.h"
#include "Effect.generated.h"

struct FEffectParam;
/**
 * 
 */
UCLASS(Abstract)
class ARPG_HUNTER_API UEffect : public UObject
{
	GENERATED_BODY()

private:
	TWeakObjectPtr<UStatComponent> TargetComp; // 효과 대상 : 약참조 소유
	FEffectParam* Param;

protected:
	TWeakObjectPtr<UStatComponent> GetTarget() { return TargetComp; }
	FEffectParam* GetParam() { return Param; }
	bool IsValid() { return TargetComp.IsValid() && Param != nullptr; }

public:
	virtual void Activate(UStatComponent* _target, FEffectParam* _param)
	{
		TargetComp = _target;
		Param = _param;
	};
	virtual void Deactivate() {};
	float GetDuration();
};

UCLASS()
class ARPG_HUNTER_API URecoverHealth : public UEffect 
{
	GENERATED_BODY()
public:
	void Activate(UStatComponent* _target, FEffectParam* _param) override;
};

UCLASS()
class ARPG_HUNTER_API URecoverSkill : public UEffect
{
	GENERATED_BODY()
public:
	void Activate(UStatComponent* _target, FEffectParam* _param) override;
};

UCLASS()
class ARPG_HUNTER_API URecoverStamina : public UEffect
{
	GENERATED_BODY()
public:
	void Activate(UStatComponent* _target, FEffectParam* _param) override;
};

UCLASS()
class ARPG_HUNTER_API UAddEffectUsingSkill : public UEffect
{
	GENERATED_BODY()
public:
	void Activate(UStatComponent* _target, FEffectParam* _param) override;
};