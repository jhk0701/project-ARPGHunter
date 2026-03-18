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
	UPROPERTY(EditAnywhere)
	uint32 Level{1};
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
	float PercentWeight{ 1.0f }; // 확률 가중치
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
	FName StartPoint;

	UPROPERTY(EditAnywhere)
	TArray<FSection> Sections;

	// 클리어 보상 지급
	UPROPERTY(EditAnywhere)
	uint32 RewardGold;
	UPROPERTY(EditAnywhere)
	uint32 RewardExp;
	UPROPERTY(EditAnywhere)
	TArray<FRewardItem> RewardItems;

	// 스테이지에서 재생할 사운드
	UPROPERTY(EditAnywhere)
	TObjectPtr<class USoundBase> BgmSource;
	
	// 스테이지에서 재생할 컷씬 LevelSequence
	// UPROPERTY(EditAnywhere)
	// TArray<TSoftObjectPtr<class ULevelSequence>> CutSceneAssets;
	UPROPERTY(EditAnywhere)
	TArray<TObjectPtr<class UCutSceneAssetData>> CutSceneAssets;
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