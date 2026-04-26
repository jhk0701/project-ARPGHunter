// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "MonsterData.generated.h"

class UMonsterConfig;
enum class ECharacterStatType : uint8;

/**
 * 
 */
USTRUCT()
struct ARPG_HUNTER_API FMonsterData : public FTableRowBase
{
	GENERATED_BODY()
public:
	// Stat
	UPROPERTY(EditAnywhere)
	TMap<ECharacterStatType, uint32> BaseStat;

	UPROPERTY(EditAnywhere)
	float MeshScale{ 1.0f };
	// AI
	// 인지 범위
	UPROPERTY(EditAnywhere)
	float RecoginitionRange{ 1000.0f }; 
	// 공격 사거리
	UPROPERTY(EditAnywhere)
	float AttackRange{ 200.0f };
	// 공격을 위한 이동 시, 적과의 거리
	UPROPERTY(EditAnywhere)
	float MoveRangeOnAttack{ 100.0f }; 
	UPROPERTY(EditAnywhere)
	float MoveSpeed{ 200.0f };
	UPROPERTY(EditAnywhere)
	float ChaseSpeed{ 400.0f };
	UPROPERTY(EditAnywhere) // 활동 범위
	float ActivityRange{ 5000.0f };
	UPROPERTY(EditAnywhere)
	uint8 PlayerActionReactProbability{ 10 }; // 플레이어의 액션에 반응할 확률

	UPROPERTY(EditAnywhere)
	TObjectPtr<UMonsterConfig> Config;
};
