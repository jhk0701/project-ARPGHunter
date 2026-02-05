// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Action.generated.h"

class UEffectData;
enum class EActionInput : uint8;
enum class EAttackDirection : uint8;

USTRUCT()
struct FActionOption
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	float Range;
	UPROPERTY(EditAnywhere)
	EAttackDirection Direction;

	UPROPERTY(EditAnywhere)
	float KnockBackStr{100.0f};
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
	uint16 StaggerDamage{ 10 };
	UPROPERTY(EditAnywhere)
	TArray<FActionOption> ArrOption;

	UPROPERTY(EditAnywhere, Category = "ActionEffect")
	TArray<TObjectPtr<UEffectData>> EffectOnStart;
	UPROPERTY(EditAnywhere, Category = "ActionEffect")
	TArray<TObjectPtr<UEffectData>> EffectOnProgress;
	UPROPERTY(EditAnywhere, Category = "ActionEffect")
	TArray<TObjectPtr<UEffectData>> EffectOnHit;
};