// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "UpgradeConfig.generated.h"

enum class EEquipmentType : uint8;
enum class ECharacterStatType : uint8;

USTRUCT()
struct FUpgradeStat 
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	TArray<ECharacterStatType> Target;
};

USTRUCT()
struct FUpgradeData
{
	GENERATED_BODY()
public:
	// 업그레이드 커브
	UPROPERTY(EditAnywhere)
	TObjectPtr<UCurveTable> Curve;

	// 재료 아이템
	UPROPERTY(EditAnywhere)
	TArray<FName> IngredientIDs;
};

/**
 * 
 */
UCLASS()
class ARPG_HUNTER_API UUpgradeConfig : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	TMap<EEquipmentType, FUpgradeStat> UpgradeStatPerEquipment;
	
	UPROPERTY(EditAnywhere)
	TArray<FUpgradeData> UpgradePerRank;
};
