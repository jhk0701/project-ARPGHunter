// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "MonsterData.generated.h"

class UAction;
enum class EMonsterType : uint8;
enum class EMonsterAttackType : uint8;
enum class EAttackDetailType : uint8;
enum class ECharacterStatType : uint8;
enum class EGimicType : uint8;

USTRUCT()
struct FMonsterAction
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	EMonsterAttackType Type;
	// 액션 가중치 : 같은 종류의 액션이 여럿일때, 확률적으로 선택하기 위한 가중치
	UPROPERTY(EditAnywhere)
	float Weight{ 1.0f };
	// 공격 간 대기 간격 : 쿨타임 용도
	UPROPERTY(EditAnywhere)
	float Interval{ 2.0f };
	UPROPERTY(EditAnywhere)
	TObjectPtr<class UAction> Action;
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
	EMonsterType Type;
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
};
