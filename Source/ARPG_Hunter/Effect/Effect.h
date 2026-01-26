// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Effect.generated.h"

struct FEffectContext
{
	// 기본형
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
	uint16 ID{ 0 };

	uint32 Value{ 0 };
	float Duration{ 0.0f };
	TWeakObjectPtr<AActor> Target; // 효과 대상 : 약참조 소유

protected:
	TWeakObjectPtr<AActor> GetTarget() { return Target; }

public:
	virtual void Activate(uint16 _id, TWeakObjectPtr<AActor> _target, FEffectContext* _context);
	virtual void Deactivate() {};

	float GetDuration() { return Duration; }
};
