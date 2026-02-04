// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "StageData.generated.h"


USTRUCT()
struct FMonsterSpawn
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	FName MonsterID;
	UPROPERTY(EditAnywhere)
	uint8 Count;
};

USTRUCT()
struct FSection
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	TArray<FMonsterSpawn> Spawn;
};

USTRUCT()
struct FRewardItem
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	FName ID;
	UPROPERTY(EditAnywhere)
	uint8 Count; // 지급 갯수
	UPROPERTY(EditAnywhere)
	uint8 PercentWeight{ 1 }; // 확률 가중치
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
	FString StageName;
	UPROPERTY(EditAnywhere)
	FString StageDesc;
	
	UPROPERTY(EditAnywhere)
	FName LevelToLoad;

	UPROPERTY(EditAnywhere)
	TArray<FSection> Sections;

	// 클리어 보상 지급
	UPROPERTY(EditAnywhere)
	uint32 RewardGold;
	UPROPERTY(EditAnywhere)
	TArray<FRewardItem> RewardItems;
};

USTRUCT(BlueprintType)
struct ARPG_HUNTER_API FRegionData : public FTableRowBase 
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	FString RegionName;
	UPROPERTY(EditAnywhere)
	TArray<FName> StageIDs;
};