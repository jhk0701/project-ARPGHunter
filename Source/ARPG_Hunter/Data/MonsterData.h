// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "MonsterData.generated.h"

UENUM(BlueprintType)
enum class EMonsterType : uint8
{
	MELEE		UMETA(DisplayName = "Melee"),
	RANGED		UMETA(DisplayName = "Ranged"),
	BOSS		UMETA(DisplayName = "Boss"),

	END			UMETA(DisplayName = "End")
};

/**
 * 
 */
USTRUCT(BlueprintType)
struct ARPG_HUNTER_API FMonsterData : public FTableRowBase
{
	GENERATED_BODY()
public:

};
