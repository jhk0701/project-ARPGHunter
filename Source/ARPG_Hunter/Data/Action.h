// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Define/Enum.h"
#include "Action.generated.h"

USTRUCT(BlueprintType)
struct FActionRange 
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	float Range;
	UPROPERTY(EditAnywhere)
	EAttackDirection Direction;
};


USTRUCT(BlueprintType)
struct FActionEffect 
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	uint32 Value;
	UPROPERTY(EditAnywhere)
	float Duration;
	UPROPERTY(EditAnywhere)
	TObjectPtr<class UEffect> Effect;
};

/**
 * 
 */
UCLASS()
class ARPG_HUNTER_API UAction : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	EActionInput InputType;
	UPROPERTY(EditAnywhere)
	uint16 StaminaUsage{ 0 };
	UPROPERTY(EditAnywhere)
	TObjectPtr<UAnimMontage> Montage;
	UPROPERTY(EditAnywhere)
	uint16 AttackDamagePer{ 100 };
	UPROPERTY(EditAnywhere)
	TArray<FActionRange> RangeArray;
	UPROPERTY(EditAnywhere)
	uint16 StaggerDamage{ 10 };

	UPROPERTY(EditAnywhere, Category = "ActionEffect")
	TArray<FActionEffect> EffectOnStart;
	UPROPERTY(EditAnywhere, Category = "ActionEffect")
	TArray<FActionEffect> EffectOnHit;
};