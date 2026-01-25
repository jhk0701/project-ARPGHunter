// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Effect.generated.h"

struct FEffectContext 
{
	uint32 EffectValue;
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
	virtual void ActivateEffect(const FEffectContext& _context) {};
};
