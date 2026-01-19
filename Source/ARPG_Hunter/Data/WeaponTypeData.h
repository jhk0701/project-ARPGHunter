// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "WeaponTypeData.generated.h"

enum class EWeaponType : uint8;

/**
 * 
 */
USTRUCT()
struct ARPG_HUNTER_API FWeaponTypeData : public FTableRowBase
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere)
	EWeaponType Type;

	UPROPERTY(EditAnywhere, Category = "Animation")
	TSubclassOf<UAnimInstance> AnimInstance;
	UPROPERTY(EditAnywhere, Category = "Animation")
	TObjectPtr<UAnimMontage> HitMontage;
	UPROPERTY(EditAnywhere, Category = "Animation")
	TObjectPtr<UAnimMontage> DodgeMontage;
	UPROPERTY(EditAnywhere, Category = "Animation|Attack")
	TObjectPtr<UAnimMontage> AttackMontage;
	
};
