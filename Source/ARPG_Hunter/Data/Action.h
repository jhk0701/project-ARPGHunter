// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Action.generated.h"

UENUM(BlueprintType)
enum class EActionInput : uint8
{
	NORMAL		UMETA(DisplayName = "Normal"),
	HOLD		UMETA(DisplayName = "Hold"),
};

USTRUCT(BlueprintType)
struct FActionEffect 
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	uint32 EffectValue;
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
	float AttackRange{ 100.0f };
	UPROPERTY(EditAnywhere)
	uint16 StaggerDamage{ 10 };

	UPROPERTY(EditAnywhere, Category = "ActionEffect")
	TArray<FActionEffect> EffectOnStart;
};