// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "MonsterConfig.generated.h"

class UAction;
enum class EMonsterType : uint8;
enum class EMonsterAttackType : uint8;

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
UCLASS()
class ARPG_HUNTER_API UMonsterConfig : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	EMonsterType Type;
	UPROPERTY(EditAnywhere)
	FName Name;

	UPROPERTY(EditAnywhere, Category = "Appearance")
	TObjectPtr<USkeletalMesh> BodyMesh;
	UPROPERTY(EditAnywhere, Category = "Appearance")
	TObjectPtr<USkeletalMesh> WeaponMesh;

	// Capsule
	UPROPERTY(EditAnywhere, Category = "Collider")
	float HalfHeight{ 88.0f };
	UPROPERTY(EditAnywhere, Category = "Collider")
	float Radius{ 34.0f };

	UPROPERTY(EditAnywhere, Category = "Effect")
	TObjectPtr<class UNiagaraSystem> VFXOnHit;

	// Action
	UPROPERTY(EditAnywhere, Category = "Action")
	TSubclassOf<UAnimInstance> AnimBP;
	UPROPERTY(EditAnywhere, Category = "Action")
	TObjectPtr<UAnimMontage> HitMontage;
	UPROPERTY(EditAnywhere, Category = "Action")
	TObjectPtr<UAnimMontage> ExtraMontage;
	UPROPERTY(EditAnywhere, Category = "Action")
	TArray<FMonsterAction> AttackActions;
};
