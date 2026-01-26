// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Effect.generated.h"

struct FEffectContext 
{
	uint32 EffectValue{0};
	float Duration{0.0f};
	AActor* Target;
};

/**
 * 
 */
UCLASS()
class ARPG_HUNTER_API UEffect : public UDataAsset
{
	GENERATED_BODY()

public:
	virtual void Activate(const FEffectContext& _context) {};
};
