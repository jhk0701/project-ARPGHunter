// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "EquipmentUpgradeData.generated.h"

enum class EEquipmentType : uint8;
enum class ECharacterStatType : uint8;

/*	
	ID 구성 규칙 rank_grade_typeStr
	ex) 
	0_0_Weapon
	0_1_Weapon
	1_0_Head...
*/

USTRUCT()
struct FUpgradeIngredient 
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	FName ItemID;
	UPROPERTY(EditAnywhere)
	uint32 Amount;
};

USTRUCT()
struct FEquipmentUpgradeData : public FTableRowBase 
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	float SuccessPercent;
	UPROPERTY(EditAnywhere)
	uint32 GoldCost;
	UPROPERTY(EditAnywhere)
	TArray<FUpgradeIngredient> Ingredients;
	UPROPERTY(EditAnywhere)
	TMap<ECharacterStatType, uint32> StatPerStep;
public:
	static FName GetUpgradeID(uint8 _rank, uint8 _grade, EEquipmentType _type);
};