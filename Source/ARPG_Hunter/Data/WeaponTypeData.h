// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "WeaponTypeData.generated.h"

enum class EWeaponType : uint8;

USTRUCT()
struct FAction
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	uint16 StaminaUsage;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UAnimMontage> Montage;
};

USTRUCT()
struct FNestedAction 
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	TArray<FAction> ActionArray;
};

/**
 * 
 */
USTRUCT()
struct ARPG_HUNTER_API FWeaponTypeData : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	EWeaponType Type;


	UPROPERTY(EditAnywhere, Category = "Animation")
	TObjectPtr<UAnimMontage> HitMontage;
	UPROPERTY(EditAnywhere, Category = "Animation")
	FAction DodgeAction;

	UPROPERTY(EditAnywhere, Category = "Animation|Attack")
	TArray<FNestedAction> AttackAction;
};