// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Define/Enum.h"
#include "MonsterData.generated.h"

class UAction;

USTRUCT()
struct FMonsterAction
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	float Weight{ 1.0f };
	UPROPERTY(EditAnywhere)
	TObjectPtr<UAction> Action;
};

/**
 * 
 */
USTRUCT()
struct ARPG_HUNTER_API FMonsterData : public FTableRowBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	EMonsterType Type{ EMonsterType::MELEE };
	UPROPERTY(EditAnywhere)
	FName Name;

	UPROPERTY(EditAnywhere)
	TObjectPtr<USkeletalMesh> BodyMesh;
	UPROPERTY(EditAnywhere)
	TObjectPtr<USkeletalMesh> WeaponMesh;

	// Capsule
	UPROPERTY(EditAnywhere)
	float HalfHeight{ 88.0f };
	UPROPERTY(EditAnywhere)
	float Radius{ 34.0f };
	UPROPERTY(EditAnywhere)
	float MeshScale{ 1.0f };

	UPROPERTY(EditAnywhere)
	TObjectPtr<class UNiagaraSystem> VFXOnHit;

	// Action
	UPROPERTY(EditAnywhere)
	TSubclassOf<UAnimInstance> AnimBP;
	UPROPERTY(EditAnywhere)
	TObjectPtr<UAnimMontage> HitMontage;
	UPROPERTY(EditAnywhere)
	TArray<FMonsterAction> AttackActions;

	// Stat
	UPROPERTY(EditAnywhere)
	TMap<ECharacterStatType, uint32> BaseStat;

	// AI
	UPROPERTY(EditAnywhere)
	float RecoginitionRange{ 1000.0f }; // 인지 범위
	UPROPERTY(EditAnywhere)
	float AttackRange{ 200.0f }; // 공격 사거리
	UPROPERTY(EditAnywhere) // 공격 간 대기 간격
	float AttackInterval{ 2.0f };
	UPROPERTY(EditAnywhere)
	float MoveRangeOnAttack{ 100.0f }; // 공격을 위한 이동 시, 적과의 거리
	UPROPERTY(EditAnywhere)
	float MoveSpeed{ 200.0f }; 
};
