// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Hitable.generated.h"

enum class EAttackType : uint8;

struct FHitInfo
{
	EAttackType AttackType;
	bool bIsCriticalHit{ false };
	bool bIgnoreDefense{ false };
	uint32 Damage{0};
	uint32 StaggerDamage{0};
	float KnockBackStrength{0.0f};
	TWeakObjectPtr<AActor> Attacker; // 공격자 약참조로 전달
	FHitResult* HitResult;
};

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UHitable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ARPG_HUNTER_API IHitable
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual uint32 HitBy(const FHitInfo& _hitInfo) = 0;
};
