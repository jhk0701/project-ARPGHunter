// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "StageData.generated.h"


USTRUCT(BlueprintType)
struct FMonsterSpawn
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	FName MonsterID;
	UPROPERTY(EditAnywhere)
	uint8 Count;
};

USTRUCT(BlueprintType)
struct FSection
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	TArray<FMonsterSpawn> Spawn;
};

/**
 * 
 */
USTRUCT(BlueprintType)
struct ARPG_HUNTER_API FStageData : public FTableRowBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	TArray<FSection> Sections;

	// 클리어 보상 지급
	// 재화 골드
	// 아이템
	// 
};
