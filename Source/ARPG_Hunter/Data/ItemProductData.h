// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "ItemProductData.generated.h"


USTRUCT()
struct FIngredient
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	FName ID;
	UPROPERTY(EditAnywhere)
	uint32 RequireAmount;
};

/**
 * 
 */
USTRUCT()
struct ARPG_HUNTER_API FItemProductData  : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	FName ItemID; // 제작할 아이템 ID
	
	UPROPERTY(EditAnywhere)
	TArray<FIngredient> Ingredients;
	UPROPERTY(EditAnywhere)
	uint32 GoldCost;
};
