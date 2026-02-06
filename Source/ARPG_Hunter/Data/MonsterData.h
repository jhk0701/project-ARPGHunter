// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Define/Enum.h"
#include "MonsterData.generated.h"

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

	UPROPERTY(EditAnywhere)
	TSubclassOf<UAnimInstance> AnimBP;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UAnimMontage> HitMontage;
	UPROPERTY(EditAnywhere)
	TArray<TObjectPtr<UAnimMontage>> AttackMontages;

	UPROPERTY(EditAnywhere)
	TMap<ECharacterStatType, uint32> BaseStat;

	UPROPERTY(EditAnywhere)
	float RecoginitionRange{ 1000.0f };
	UPROPERTY(EditAnywhere)
	float AttackRange{ 200.0f };
	UPROPERTY(EditAnywhere)
	float MoveSpeed{ 200.0f };
};
