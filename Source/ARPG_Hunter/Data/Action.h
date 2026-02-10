// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Action.generated.h"

class UEffectData;
enum class EActionInput : uint8;
enum class EAttackDirection : uint8;
enum class EAttackType : uint8;

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
	EAttackType Type;

	UPROPERTY(EditAnywhere)
	uint16 AttackDamagePer{ 100 };
	UPROPERTY(EditAnywhere)
	uint16 StaggerDamage{ 10 };
	UPROPERTY(EditAnywhere, Category = "Appearance|Motion")
	TObjectPtr<UAnimMontage> Montage;
	UPROPERTY(EditAnywhere, Category = "Appearance|Motion")
	TArray<FActionOption> ArrOption;
	UPROPERTY(EditAnywhere, Category = "Appearance|Effect")
	TObjectPtr<class UNiagaraSystem> VFXOnHit;

	UPROPERTY(EditAnywhere, Category = "Setting")
	EActionInput InputType;
	UPROPERTY(EditAnywhere)
	uint8 StaminaUsage{ 0 };

	UPROPERTY(EditAnywhere, Category = "ActionEffect")
	TArray<TObjectPtr<UEffectData>> EffectOnStart;
	UPROPERTY(EditAnywhere, Category = "ActionEffect")
	TArray<TObjectPtr<UEffectData>> EffectOnProgress;
	UPROPERTY(EditAnywhere, Category = "ActionEffect")
	TArray<TObjectPtr<UEffectData>> EffectOnHit;
	UPROPERTY(EditAnywhere, Category = "ActionEffect")
	TArray<TObjectPtr<UEffectData>> EffectOnEnemyHit;
};