// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Define/Enum.h"
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
USTRUCT()
struct ARPG_HUNTER_API FMonsterData : public FTableRowBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EMonsterType Type{ EMonsterType::MELEE };
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName Name;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<USkeletalMesh> Mesh;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UAnimBlueprint> AnimBP;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UAnimMontage> HitMontage;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<TObjectPtr<UAnimMontage>> AttackMontages;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TMap<ECharacterStatType, int32> BaseStat;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float RecoginitionRange{ 1000.0f };
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float AttackRange{ 200.0f };
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float MoveSpeed{ 200.0f };
};
