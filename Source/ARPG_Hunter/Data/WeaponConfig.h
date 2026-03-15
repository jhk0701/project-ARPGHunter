// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "WeaponConfig.generated.h"

enum class EWeaponType : uint8;

/**
 * 
 */
UCLASS()
class ARPG_HUNTER_API UWeaponConfig : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "Animation")
	TObjectPtr<UAnimMontage> HitMontage;
	UPROPERTY(EditAnywhere, Category = "Animation")
	TObjectPtr<UAnimMontage> ItemUsageMontage;
	UPROPERTY(EditAnywhere, Category = "Animation")
	TObjectPtr<class UAction> DodgeAction;
	
	UPROPERTY(EditAnywhere, Category = "Attack Combo")
	TObjectPtr<class UActionComboData> AttackCombo;
	UPROPERTY(EditAnywhere, Category = "Skill Tree")
	TObjectPtr<class USkillTreeData> SkillTree;
};

UCLASS()
class ARPG_HUNTER_API UWeaponType : public UDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	TMap<EWeaponType, TObjectPtr<UWeaponConfig>> Map;
};
